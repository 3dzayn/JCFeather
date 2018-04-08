#ifndef _jcFeather_
#define _jcFeather_

#include <maya/MPxLocatorNode.h>
#include <maya/MFloatPointArray.h>
#include <maya/MMatrix.h>
#include <maya/MFloatArray.h>
#include <maya/MColorArray.h>
#include <maya/MPlugArray.h>

#include <vector>
#include <map>
#include "featherInfo.h"
#include "HashTable.h"
class singleFeather;

class jcFeather : public MPxLocatorNode
{
	friend class featherGen;
	friend class jcFeatherToCurves;
	public:
						jcFeather();
		virtual         ~jcFeather();
		virtual void	postConstructor(); 
		MStatus         initRampData(  MObject& rampObj, 
												int index, 
												float position, 
												float value, 
												int interpolation);
		void			createDefaultRampData();

		virtual MStatus compute( const MPlug&, MDataBlock& );
		MStatus         connectionMade ( const MPlug & plug, const MPlug & otherPlug, bool asSrc );
		MStatus         connectionBroken ( const MPlug & plug, const MPlug & otherPlug, bool asSrc );
		void            updateJCFTexture();


		void            addPlugTurtleData(const MPlug &plug,bool surface,std::map<int,long> &indexMap);
		void            removePlugTurtleData(const MPlug &plug,bool surface,std::map<int,long> &indexMap);
		bool            makeFeathers(MDataBlock&);

		void            draw(M3dView &view, 
							 const MDagPath &path, 
							 M3dView::DisplayStyle style, 
							 M3dView::DisplayStatus);
		bool			isBounded() const{return true;}
		MBoundingBox    boundingBox() const;
		bool excludeAsLocator() const{return false;}

		static  void *  creator();
		static  MStatus initialize();
		static  void    addAffect();
		static void     removeTurtlesDataBaseKey(long* id);
		static void     removeFeaInfoDataBase(long* id);
		static void     nodeRemoved(MObject& node, void *clientData);
		static void     getActiveJCFeatherNode(MObjectArray &featherAry,bool visibleOnly);

		void    cleanOutMesh(MDataBlock& block);

	public:
		static  MTypeId id;
		static  MString typeName;
		// Input attributes
		//---------------------------------------------------------------------------shape attr
		static MObject  displayPercent;  //opengl��ʾ�ٷֱ�  int
		static MObject  displayAs;
		static MObject  uniformWidth;
		static MObject  render;
		static MObject  exactFeather;
		static MObject  active;//�Ƿ���м��� bool
		static MObject  direction;//��������  enum
		static MObject  curveType;//���߶���  enum

		static MObject  rachisRenderAs;
		static MObject  rachisSegment;   //�������     int
		static MObject  rachisPosition;
		static MObject  rachisCrossSection; //int
		static MObject  rachisThick;     //����ֶ�     double
		static MObject  rachisStart;
		static MObject  rachisEnd;
		static MObject  rachisThickScale;//����ֶ����� rampAttribute

		static MObject  barbuleNum;      //��֦���������ҶԳƣ�������� int
		static MObject  barbuleDensity;
		static MObject  forcePerSegment;    //ÿ���ۼӵ�������� rand(max) 
		static MObject  turnForce;        //�����ۼӵ���ֵת��  double

		static MObject  rotateLamda; //ÿ��ת�ǵ�������������� double
		static MObject  upDownLamda;   //ÿ�������ƶ���������ȵ����� double
		
		static MObject  rachisNoiseFrequency;
		static MObject  barbuleNoiseFrequency;
		static MObject  noisePhase;
		
		static MObject  shapeSymmetry;
		static MObject  barbuleSegments; //int  
		static MObject  barbuleLength;   //��֦����      double

		static MObject  gapForce;
		static MObject  gapMaxForce;
		static MObject  gapSize;
		static MObject  leftGapDensity;
		static MObject  rightGapDensity;
		
		static MObject  forceScale;
		static MObject  forceRotateScale;
		static MObject  barbuleLengthRandScale; //��֦����������� double
		static MObject  leftBarbuleLengthScale;  
		static MObject  rightBarbuleLengthScale;   //��֦��������      rampAttribute

		static MObject  upDownNoise;
		static MObject  barbuleUpDownScale;//along barbule
		static MObject  barbuleUpDownRachis;//along rachis

		static MObject  keyBarbulePos1;
		static MObject  keyBarbulePos2;
		static MObject  keyBarbulePos3;
		static MObject  keyBarbulePos4;

		static MObject  keyBarbuleStartAngle1;
		static MObject  keyBarbuleStartAngle2;
		static MObject  keyBarbuleStartAngle3;
		static MObject  keyBarbuleStartAngle4;

		static MObject  keyBarbuleStepAngle;

		static MObject  kbRotate1;
		static MObject  kbRotate2;
		static MObject  kbRotate3;
		static MObject  kbRotate4;

		static MObject  barbuleThick;    //��֦�ֶ�      double
		static MObject  barbuleThickScale;//��֦�ֶ����� rampAttribute
		static MObject  barbuleRandThick;
		static MObject  barbuleThickAdjust;

		static MObject  displayProxyBoundingBox;
		static MObject  proxyBoundingBoxScale;

		static MObject  randSeed;    //�������  int

		//---------------------------------------------------------------------------material attr
		static MObject  useOutShader;
		static MObject  projectTexUV;
		static MObject  uvProjectScale;
		static MObject  autoConvertTex;
		static MObject  barbuleColorTex;
		static MObject  textureProcedure;
		static MObject  interactiveTexture;
		static MObject  mainColor;
		static MObject  barbuleDiffuse;
		static MObject  barbuleSpecular;
		static MObject  barbuleSpecularColor;
		static MObject  barbuleGloss;
		static MObject  selfShadow;
		static MObject  barbuleHueVar;
		static MObject  barbuleSatVar;
		static MObject  barbuleValueVar;
		static MObject  varFrequency;

		static MObject  rootColor;  //��֦ ������ɫ
		static MObject  tipColor;  //��֦ �ⲿ��ɫ
		static MObject  baseOpacity;
		static MObject  fadeOpacity;
		static MObject  fadeStart;
		static MObject  rachisRootColor;
		static MObject  rachisTipColor;

		//----------------------------------------------------------------------------renderman content
		static MObject  preFeatherRib;    
		static MObject  preFeatherMeshRib;
		static MObject  postFeatherRib;    
		static MObject  postFeatherMeshRib;
		
		static MObject  guidSurfaceInfo;
		static MObject  guidSurface;
		static MObject  guidUV;

		static MObject  inFeatherTurtleData;
		static MObject  inFeatherTurtleArrayData;

		// Output attributes
		static MObject  outputMesh;
		static MObject  outRenderFeather;


		static MObject  outputFeatherMesh;
		static MObject  meshUVScale;
		
		//output rachis mesh and barbule mesh
		static MObject  outRachisMesh;
		static MObject  outBarbuleMesh;

	private:
		long *featherID;
		featherInfo *featherData;
		std::map<int,long> surfaceIndexIDMap;//��¼ÿ��guidsurface��index��Ӧ��turtleID
		std::map<int,long> inTurtleIDMap;
		std::map<std::pair<int,int>,long> inTurtleArrayIDMap;
		std::vector< singleFeather > featherDraw;
		MMatrix           m_worldMatrix;

		MColorArray mainColorArray;
		MFloatArray jcU;
		MFloatArray jcV;
};
#endif
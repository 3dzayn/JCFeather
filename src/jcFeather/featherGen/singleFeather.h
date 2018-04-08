#ifndef _singleFeather_
#define _singleFeather_

#include <vector>
#include <maya/MColor.h>
#include <maya/MColorArray.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MBoundingBox.h>
#include <maya/MIntArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MString.h>
#include <maya/MFnMesh.h>
#include <maya/MStatus.h>
#include <maya/MFnMeshData.h>
#include <maya/MFloatMatrix.h>
#include <fstream>
#if defined(_OPENMP)
	#include <omp.h>
#endif
using namespace std;

extern bool jcFea_UseMultiThread; 
extern int jcFea_MaxThreads;
extern int jcFea_ProcessorNum;

typedef std::vector< MFloatPointArray > fpa;
typedef std::vector< MFloatVectorArray > fva;

class polyObj
{
	public:
		polyObj();
		~polyObj();
		void init();

		void initWith(const polyObj &other);
		bool initWithMObject(const MObject &meshData, const MString *uvset);
		bool convertToMObject(MObject &meshData,const MString *uvset);
		bool convertToDagMObject(MObject &meshData,const MString *uvset);
		void appendMesh(const polyObj &mesh);
		void transform(const MFloatMatrix &matrix);
	public:
		MIntArray faceCounts;
		MIntArray faceConnects;
		MFloatPointArray pa;
		MFloatArray ua;
		MFloatArray va;
		MIntArray uvids;
};  //���崫��Ķ�������ݸ�ʽ

class singleFeather
{
	public:
		singleFeather();
		~singleFeather();
		void clear();

	public:
		MFloatPointArray rachis;  //rachis�ĵ��λ��
		MFloatArray rachisWidth;  //rachis�Ĵ�ϸ��������rachis�Ķ����������й�
		MFloatVectorArray rachisNormal;
		float rachisLength;//rachis���ܳ���

		fpa leftBarbules;//��¼��ÿ��leftbarbule��ÿ�����λ������
		MFloatArray leftBarbuleRachisPos;//ÿһ��barbule��rachis�ϵ�λ�õİٷֱȣ�ģ��uֵ�����Ժ���ͼ����ʹ��
		MFloatArray leftBarbuleLenPer;//��¼��ÿ��leftbarbule���������Ǹ�barbule��ռ�İٷֱ�,������shader�н�����ͼ����uv
		fva leftBarbuleNormal;//��¼��ÿ��leftbarbule��ÿ����ķ������ݣ������surface feather����洢�����������ķ��ߣ������turtlefeather,����turtle��λ����
		MFloatArray leftBarbuleWidth;//��¼��ÿһ��leftbarbule�Ĵ�ϸ���ţ���һ��barbuleһ��float��ֵ
		
		fpa rightBarbules;
		MFloatArray rightBarbuleRachisPos;
		MFloatArray rightBarbuleLenPer;
		fva rightBarbuleNormal;
		MFloatArray rightBarbuleWidth;//��¼��ÿһ��rightbarbule�Ĵ�ϸ���ţ���һ��barbuleһ��float��ֵ
		
		MFloatArray barbuleWidth;//����ÿһ��barbule�Ĵ�ϸ����ͬ,������barbule�Ķ����Լ�barbule�������йأ���¼����������barbule����ÿ����Ĵ�ϸ�仯
		
		float surfaceUV[2];//�����feather����ĳ�������ϣ����¼�ڸ������uvλ��
		MColor mainColor;//���ڲ�����ɫ��ʹÿһ����ë�б仯
		MString colorTexOverride;//ÿ����ë��������һ����ͼ�����ǵ�jcFeather�ڵ������ͼ,Ŀǰ������jcFeather��jcFeatherSystem�ڵ�������mel�������õ���ͬ��tex
		unsigned int randSeed;//�������

		MBoundingBox proxyBBox;
		MBoundingBox rachisMeshBBox;
		polyObj rachisCylinder;
		polyObj barbuleFace;
};

#endif
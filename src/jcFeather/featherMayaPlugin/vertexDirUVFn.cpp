#include "vertexDirUVFn.h"

vertexDirUVFn::vertexDirUVFn(){init();}
vertexDirUVFn::~vertexDirUVFn(){}
void vertexDirUVFn::init()
{
	_guidsPerVertex = 2;
	_searchCurveRadius = 1;
	_curvePower = 0.6f;
	_inGuidCurves.clear();
	_inGrownMesh  = MObject::kNullObj;
	_growComponent = MObject::kNullObj;
	_tolerance=0.001f;
	_maxAngle = 10;
}

void vertexDirUVFn::getVertexDir( verDirVector &outVertexGuid,
								  vector<pointRay> &displayLine)
{
	MStatus status=MS::kSuccess;
	MFnMesh growMeshFn(_inGrownMesh,&status);

	//--------------��������ģ���Լ����id�ŵõ����洢��������id����ʲô��û�����ʾ����ģ�Ͷ���������
	MIntArray rawVertices;
	featherTools::getComponentIds(growMeshFn,_growComponent,rawVertices,MFn::kMeshVertComponent);

	//----------��ÿ������в�ѯ����ķ���
	int realNum = rawVertices.length();

	MPoint vertexPos,closestPt,currentVertex;
	MVector currentDir;
	MIntArray vertAry;
	int preIndex=0;
	bool findClosest=true;

	MPointArray allPts;
	status = growMeshFn.getPoints(allPts,MSpace::kWorld);
	
	MItMeshVertex  vertexIt(_inGrownMesh,&status);
	
	vertexDirectionInfo currentVD;
	pointRay currentPR;
	outVertexGuid.clear();
	displayLine.clear();
	if(_inGuidCurves.length()!=0)
	{
		for( int ii= 0;ii<realNum;++ii)
		{
			currentVD._vertexId = rawVertices[ii];
			if(!getClosestDirection(allPts[currentVD._vertexId],currentDir))
				continue;
		
			vertexIt.setIndex(currentVD._vertexId,preIndex);
			vertexIt.getConnectedVertices(vertAry);//��ȡ�͵�ǰ�����ӵĵ��id��
			if( !getClosestConnectedVertex( vertAry,
											currentDir,
											allPts,
											allPts[currentVD._vertexId],
											currentVD._dirVertexId
											))
				continue;

			currentPR._startPt = allPts[currentVD._vertexId];
			currentPR._endPt = currentPR._startPt + ( allPts[currentVD._dirVertexId] - currentPR._startPt )/2;

			outVertexGuid.push_back(currentVD);
			displayLine.push_back(currentPR);
		}
	}
	else //if no in guid curves, use the max connected id vertex.
	{
		for( int ii= 0;ii<realNum;++ii)
		{
			currentVD._vertexId = rawVertices[ii];
		
			vertexIt.setIndex(currentVD._vertexId,preIndex);
			vertexIt.getConnectedVertices(vertAry);//��ȡ�͵�ǰ�����ӵĵ��id��
			
			currentVD._dirVertexId=-1;
			for(unsigned int jj=0;jj<vertAry.length();++jj)
				if(vertAry[jj]>currentVD._dirVertexId)
					currentVD._dirVertexId = vertAry[jj];

			currentPR._startPt = allPts[currentVD._vertexId];
			currentPR._endPt = currentPR._startPt + ( allPts[currentVD._dirVertexId] - currentPR._startPt )/2;

			outVertexGuid.push_back(currentVD);
			displayLine.push_back(currentPR);
		}	
	}
}

bool vertexDirUVFn::getClosestDirection(const MPoint &pt,MVector &dir)//���ҳ���ֵ�����������е����߷���
{
	MStatus status=MS::kSuccess;

	MFnNurbsCurve curveFn;

	unsigned int curveSize = _inGuidCurves.length();
	double paramValue=0,dist=0;
	MPoint closestPos;
	std::vector<jerryC::idDist> idDistArray;
	jerryC::idDist currentIDDist;
	MVectorArray tangentA;
	MVector currentTangent;
	//--------���ҳ��ڸ������뷶Χ�ڵ����е������
	for(unsigned int ii=0;ii<curveSize;++ii)
	{
		curveFn.setObject(_inGuidCurves[ii]);
		curveFn.closestPoint(pt,&paramValue,_tolerance,MSpace::kWorld,&status);
		curveFn.getPointAtParam(paramValue,closestPos,MSpace::kWorld);
		dist = (closestPos-pt).length();
		if((float)dist <= _searchCurveRadius)
		{
			currentTangent = curveFn.tangent(paramValue,MSpace::kWorld);
			tangentA.append(currentTangent);
			currentIDDist._id = tangentA.length()-1;
			currentIDDist._distance = (float) dist;
			idDistArray.push_back(currentIDDist);
		}
	}

	//----����
	unsigned int idsize = (unsigned int)idDistArray.size();
	if(idsize==0)return false;
	jerryC::idDistSort(idDistArray,0,idsize-1);

	//----�����ֵ
	unsigned int realsize=_guidsPerVertex;
	if(realsize>idsize) realsize = idsize;
	dir = MVector(0,0,0);
	float gaussExp=0;
	for(unsigned int ii=0;ii<realsize;++ii)
	{
		gaussExp =  exp( -idDistArray[ii]._distance *  idDistArray[ii]._distance *  _curvePower*_curvePower);
		if(gaussExp<0.0000001f) gaussExp=0.0000001f;
		dir += gaussExp * tangentA[idDistArray[ii]._id];
	}
	dir.normalize();

	return true;
}

bool vertexDirUVFn::getClosestConnectedVertex(  const MIntArray &verIdAry,
												const MVector &curveDir,
												const MPointArray &allPts,
												const MPoint &pt,
												int &dirVerid)
{
	double tempAngle=0,minAngle=3.2;
	MVector vertexDir;
	for(int jj=0;jj<verIdAry.length();++jj)
	{
		vertexDir = allPts[verIdAry[jj]] - pt;
		tempAngle = curveDir.angle(vertexDir);
		if(tempAngle<minAngle)
		{
			dirVerid = verIdAry[jj];
			minAngle = tempAngle;
		}
	}
	if(minAngle>_maxAngle)return false;
	else return true;
}

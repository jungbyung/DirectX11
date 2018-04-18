#pragma once
#include <fbxsdk.h>

class Mesh;
class Bone;
class Subset;
struct KeyFrame
{
	UINT mFrameNum;
	XMFLOAT4X4 mMat;
	KeyFrame* next;
	FLOAT mTime;

	KeyFrame() : next(nullptr), mTime(0.0f), mFrameNum(0) {}
};
struct Joint
{
	string mName;
	INT mParentIndex;
	XMFLOAT4X4 mMat;
	map<string, KeyFrame*> mMapAnimation;
	map<string, int> mMapLength;
	KeyFrame* mAnimation;
	INT mLength;
};

struct Skeleton
{
	vector<Joint> mVecJoint;
};

struct VertexInfo
{
	XMFLOAT3 mPos;
	XMFLOAT3 mNormal;
	XMFLOAT2 mUV;
	XMFLOAT3 mTangentU;
	FLOAT mWeight[3];
	BYTE mIndices[4];
};

class FBXLoader : public Singleton<FBXLoader>
{
public:
	FBXLoader();
	~FBXLoader();
private:
	Bone* ParseSkeletonHierarchy(FbxNode* pNode);
	VOID ParseSkeletonHierarchyRecursively(FbxNode * pNode, int inDepth, int myIndex, int inParentIndex, Bone* bone, Bone* p = nullptr);

	VOID ParseNode(FbxNode* pNode, vector<Subset*>& subsets);
	VOID ParseControlPoint(FbxNode* pNode, vector<JB::Vertex>& ver);
	VOID ParseVertexInfo(FbxNode* pNode, vector<JB::Vertex>& ver, vector<Subset*>& subsets);
	VOID ParseMaterial(FbxNode* pNode, vector<Subset*>& subsets);

	void ParseMaterialAttribute(FbxSurfaceMaterial* inMaterial, UINT index, Subset* inObj);
	void ParseMaterialTexture(FbxSurfaceMaterial* inMaterial, UINT index, Subset* inObj);

	VOID ReadNormal(FbxMesh* pMesh, UINT mPolygonIndex, UINT IndexNum, JB::Vertex& pnt);
	VOID ReadUV(FbxMesh* pMesh, UINT mPolygonIndex, UINT UVIndex, JB::Vertex& pnt);
	VOID ReadTangent(FbxMesh* pMesh, UINT mPolygonIndex, UINT IndexNum, JB::Vertex& pnt);

public:
	Mesh* LoadFBX(const string fileName);
};

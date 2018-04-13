#pragma once
#include <fbxsdk.h>

class Mesh;

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

struct Subset
{
	vector<VertexInfo> mVecVertices;
	vector<UINT> mVecIndices;
	string mDiffuseTexture;
	string mSpecularTexture;
	string mNormalTexture;

	Material mMaterial;
};

class FBXLoader
{
private:
	vector<VertexInfo> mVecPNT;

	Skeleton mSkeleton;
	vector<string> mVecName;

	map<int, Subset> mMapSubset;

	bool IsBoneAnimation;

	static FbxManager* mFbxSdkManager;
	Mesh* mMesh;
public:
	FBXLoader();
	~FBXLoader();

private:
	VOID ParseSkeletonHierarchy(FbxNode* pNode);
	VOID ParseSkeletonHierarchyRecursively(FbxNode * pNode, int inDepth, int myIndex, int inParentIndex);

	VOID ParseNode(FbxNode* pNode);
	VOID ParseControlPoint(FbxNode* pNode);
	VOID ParseVertexInfo(FbxNode* pNode);
	VOID ParseMaterial(FbxNode* pNode);

	void ParseMaterialAttribute(FbxSurfaceMaterial* inMaterial, UINT index, Subset* inObj);
	void ParseMaterialTexture(FbxSurfaceMaterial* inMaterial, UINT index, Subset* inObj);

	VOID ReadNormal(FbxMesh* pMesh, UINT mPolygonIndex, UINT IndexNum, VertexInfo& pnt);
	VOID ReadUV(FbxMesh* pMesh, UINT mPolygonIndex, UINT UVIndex, VertexInfo& pnt);
	VOID ReadTangent(FbxMesh* pMesh, UINT mPolygonIndex, UINT IndexNum, VertexInfo& pnt);

public:
	static BOOL LoadFBX(const string fileName);
};

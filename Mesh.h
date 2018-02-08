#pragma once
#include "Object.h"

class Bone
{
public:
	Bone() {}
	~Bone()
	{
		SafeDelete(mParent);
		vector<Bone*>::iterator iter = mChilds.begin();
		for (iter; iter != mChilds.end(); ++iter)
		{
			SafeDelete(*iter);
		}
	}

	VOID Initialize(string name, int index, CXMMATRIX mat = XMMatrixIdentity(), Bone* parent = nullptr)
	{
		mName = name;
		mIndex = index;
		mMat = mat;
		mParent = parent;
	}

	Bone* GetChild(int index) const
	{
		if (mChilds.size() >= index) return nullptr;

		return mChilds[index];
	}

	Bone* GetParent() { return mParent; };
	XMMATRIX GetMatrix() { return mMat; }
	INT GetIndex() { return mIndex; }
	string GetName() { return mName; }

private:
	string mName;
	INT mIndex;
	XMMATRIX mMat;

	Bone* mParent;
	vector<Bone*> mChilds;
};

class Subset : public Object
{
public:
	Subset() {}
	~Subset()
	{
		ReleaseCOM(mVB);
		ReleaseCOM(mIB);
		ReleaseCOM(mDiffuseMapSRV);
		ReleaseCOM(mSpecularMapSRV);
		ReleaseCOM(mNormalMapSRV);
	}

private:

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	ID3D11ShaderResourceView* mDiffuseMapSRV;
	ID3D11ShaderResourceView* mSpecularMapSRV;
	ID3D11ShaderResourceView* mNormalMapSRV;

	//vector<Vertex> mVertices;
	vector<UINT> mIndices;
	string mDiffuseTextureName;
	string mSpecularTextureName;
	string mNormalTextureName;

	//Material mMaterial;
};

class Mesh : public Object
{
public:
	Mesh();
	~Mesh();

	virtual void Update();
	virtual void Draw();
private:
	vector<Subset> mSubset;
	Bone* mBone;
};

#pragma once
#include "Object.h"

class Bone
{
public:
	Bone() {}
	~Bone()
	{
		//SafeDelete(mParent);
		//vector<Bone*>::iterator siter = mChilds.begin();
		//vector<Bone*>::iterator eiter = mChilds.end();
		//for (siter; siter != eiter; ++siter)
		//{
		//	SafeDelete(*siter);
		//}
	}

	VOID Initialize(string name, int index, CXMMATRIX mat = XMMatrixIdentity(), Bone* parent = nullptr)
	{
		mName = name;
		mIndex = index;
		XMStoreFloat4x4(&mMat, mat);
		mParent = parent;
	}

	Bone* GetChild(int index) const
	{
		if (mChilds.size() <= index) return nullptr;

		return mChilds[index];
	}

	void AddChild(Bone* bone)
	{
		mChilds.push_back(bone);
	}

	bool FindStringOfBone(const string& str)
	{
		if (mName.compare(str) == 0)
			return true;
		return false;
	}

	UINT GetChildNum() const
	{
		return mChilds.size();
	}

	void SetMatrix(CXMMATRIX mat)
	{
		XMStoreFloat4x4(&mMat, mat);
	}
	Bone* GetParent() { return mParent; };
	const XMFLOAT4X4& GetMatrix() { return mMat; }
	INT GetIndex() { return mIndex; }
	string GetName() { return mName; }
private:
	string mName;
	INT mIndex;
	XMFLOAT4X4 mMat;

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
	vector<Vertex>& GetVertices() { return mVertices; }
	vector<UINT>& GetIndices() { return mIndices; }

	void SetMaterial(Material mat) { mMaterial = mat; }

	void SetTextureName(const int n, const string str)
	{
		switch (n)
		{
		case 0:
			mDiffuseTextureName = str;
			break;
		case 1:
			mSpecularTextureName = str;
			break;
		case 2:
			mNormalTextureName = str;
			break;
		}
	}

	ID3D11ShaderResourceView* GetMapSRV(int n)
	{
		switch (n)
		{
		case 0:
			return mDiffuseMapSRV;
			break;
		case 1:
			return mSpecularMapSRV;
			break;
		case 2:
			return mNormalMapSRV;
			break;
		}
		return nullptr;
	}
	Material GetMaterial() { return mMaterial; }
	void operVertexShader(vector<XMFLOAT4X4> mat);
	void Initialize(ID3D11Device * pDevice);
	virtual void Update(const float deltaTime);
	void Draw(ID3D11DeviceContext * dc);

private:

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	ID3D11ShaderResourceView* mDiffuseMapSRV;
	ID3D11ShaderResourceView* mSpecularMapSRV;
	ID3D11ShaderResourceView* mNormalMapSRV;

	vector<Vertex> mVertices;
	vector<UINT> mIndices;
	string mDiffuseTextureName;
	string mSpecularTextureName;
	string mNormalTextureName;

	Material mMaterial;
};

class Mesh : public Object
{
public:
	Mesh();
	~Mesh();

	void Initialize(ID3D11Device * pDevice);
	virtual void Update(const float deltaTime);
	void Draw(ID3D11DeviceContext * dc, CXMMATRIX viewproj);

	Bone* GetBone() { return mBone; }
	void SetBone(Bone* bone) { mBone = bone; }

	vector<Subset*>& GetSubset() { return mSubset; }

	void out();
private:
	vector<Subset*> mSubset;
	Bone* mBone;
	vector<XMFLOAT4X4> mTransform;
};

class StaticMesh : public Object
{
public:
	StaticMesh();
	~StaticMesh();

private:
	vector<Subset*> mSubsets;
};

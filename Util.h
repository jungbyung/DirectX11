#pragma once

class Bone;
namespace JB
{
	struct PC
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
	};

	struct PNT
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};

	struct Vertex
	{
		XMFLOAT3 mPos;
		XMFLOAT3 mNormal;
		XMFLOAT2 mUV;
		XMFLOAT4 mTangentU;
		FLOAT mWeight[3];
		BYTE mIndices[4];
	};
	struct Material
	{
		Material() { ZeroMemory(this, sizeof(this)); }

		XMFLOAT4 mAmbient;
		XMFLOAT4 mDiffuse;
		XMFLOAT4 mSpecular;
		XMFLOAT4 mReflect;
	};

	struct DirectionalLight
	{
		DirectionalLight() { ZeroMemory(this, sizeof(this)); }
		XMFLOAT4 mAmbient;
		XMFLOAT4 mDiffuse;
		XMFLOAT4 mSpecular;

		XMFLOAT3 Direction;
	};

	struct PointLight
	{
		PointLight() { ZeroMemory(this, sizeof(this)); }

		XMFLOAT4 mAmbient;
		XMFLOAT4 mDiffuse;
		XMFLOAT4 mSpecular;

		XMFLOAT3 mPos;
		float mRange;
	};

	HRESULT Line(ID3D11Device* pDevice, ID3D11DeviceContext* dc, XMFLOAT3 x1, XMFLOAT3 x2, XMFLOAT4 color, CXMMATRIX ViewProj);

	XMMATRIX InverseTranspose(CXMMATRIX M);

	void FindStringToBone(const string & JointName, IN Bone* bone, OUT Bone** b);
	void AddVectorTransform(Bone* bone, vector<XMFLOAT4X4>& m);
	void OutBoneName(FILE** fp, Bone* bone);

	template<class T1, class T2>
	void AddT1ToT2(T1* t1, vector<T2>& t2)
	{
		t2.push_back(t1);
	}
};

class Util
{
public:
	Util();
	~Util();
};

class LayoutDesc
{
public:
	static const D3D11_INPUT_ELEMENT_DESC pc[2];
	static const D3D11_INPUT_ELEMENT_DESC pt[2];
	static const D3D11_INPUT_ELEMENT_DESC pnt[3];
	static const D3D11_INPUT_ELEMENT_DESC pnts[6];
};

class Layout
{
public:
	Layout();
	~Layout();

	static VOID Init(ID3D11Device* pDevice);

	static ID3D11InputLayout* mPC;
	static ID3D11InputLayout* mPT;
	static ID3D11InputLayout* mPNT;
	static ID3D11InputLayout* mPNTS;
};

template<class T>
class Singleton
{
private:
	static T* Instance;
public:
	Singleton();
	~Singleton();

	static T* GetSingleton();
	void deleteSingleton();
};

template<class T>
T* Singleton<T>::Instance = nullptr;

template<class T>
inline Singleton<T>::Singleton()
{
}

template<class T>
inline Singleton<T>::~Singleton()
{
}

template<class T>
inline T * Singleton<T>::GetSingleton()
{
	if (Instance) Instance = new T;
	return Instance;
}

template<class T>
inline void Singleton<T>::deleteSingleton()
{
	if (Instance)
		SafeDelete(Instance);
}

#pragma once

namespace JB
{
	//struct Vertex
	//{
	//	XMFLOAT3 mPos;
	//	XMFLOAT3 mNormal;
	//	XMFLOAT2 mUV;
	//	XMFLOAT3 mTangentU;
	//	FLOAT mWeight[3];
	//	BYTE mIndices[4];
	//};
	//struct Material
	//{
	//	Material() { ZeroMemory(this, sizeof(this)); }
	//
	//	XMFLOAT4 mAmbient;
	//	XMFLOAT4 mDiffuse;
	//	XMFLOAT4 mSpecular;
	//	XMFLOAT4 mReflect;
	//};
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
};

class Layout
{
public:
	Layout();
	~Layout();

	static VOID Init(ID3D11Device* pDevice);

	static ID3D11InputLayout* mPC;
};
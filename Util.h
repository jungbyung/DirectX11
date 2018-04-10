#pragma once

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
		XMFLOAT3 mTangentU;
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

	struct DirectionLight
	{
		DirectionLight() { ZeroMemory(this, sizeof(this)); }
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
};
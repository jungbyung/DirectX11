#include "stdafx.h"
#include "Util.h"

Util::Util()
{
}

Util::~Util()
{
}

const D3D11_INPUT_ELEMENT_DESC LayoutDesc::pc[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
const D3D11_INPUT_ELEMENT_DESC LayoutDesc::pt[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

ID3D11InputLayout* Layout::mPC = nullptr;
ID3D11InputLayout* Layout::mPT = nullptr;

Layout::Layout()
{
}

Layout::~Layout()
{
	ReleaseCOM(mPC);
	ReleaseCOM(mPT);
}

VOID Layout::Init(ID3D11Device * pDevice)
{
	D3DX11_PASS_DESC passDesc;
	Effects::PointEffectFX->mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	pDevice->CreateInputLayout(LayoutDesc::pc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mPC);

	Effects::TextureEffectFX->mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	pDevice->CreateInputLayout(LayoutDesc::pt, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mPT);
}

HRESULT JB::Line(ID3D11Device * pDevice, ID3D11DeviceContext * dc, XMFLOAT3 x1, XMFLOAT3 x2, XMFLOAT4 color, CXMMATRIX ViewProj)
{
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	PC v[] =
	{
		{x1, color},
		{x2, color}
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(PC) * 2;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = v;

	pDevice->CreateBuffer(&vbd, &vinitData, &mVB);

	UINT i[] =
	{
		0, 1
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 2;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = i;

	pDevice->CreateBuffer(&ibd, &iinitData, &mIB);
	dc->IASetInputLayout(Layout::mPC);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	LPD3D11EFFECTTECHNIQUE activeTech = Effects::PointEffectFX->mTech;

	UINT stride = sizeof(PC);
	UINT offset = 0;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMMATRIX M = ViewProj;

	Effects::PointEffectFX->SetWroldViewProj(M);

	dc->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	dc->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	for (UINT p = 0; p < techDesc.Passes; p++)
	{
		activeTech->GetPassByIndex(p)->Apply(0, dc);
		dc->DrawIndexed(2, 0, 0);
	}
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	return S_OK;
}
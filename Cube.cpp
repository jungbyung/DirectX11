#include "stdafx.h"
#include "Cube.h"

Cube::Cube()
{
}

Cube::~Cube()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

HRESULT Cube::Init(ID3D11Device * pDevice)
{
	PC v[8];

	float r = 0.5f;

	v[0] = { XMFLOAT3(-r, -r, -r), XMFLOAT4(0,1,0,1) };
	v[1] = { XMFLOAT3(-r, +r, -r), XMFLOAT4(0,1,0,1) };
	v[2] = { XMFLOAT3(+r, +r, -r), XMFLOAT4(0,1,0,1) };
	v[3] = { XMFLOAT3(+r, -r, -r), XMFLOAT4(0,1,0,1) };
	v[4] = { XMFLOAT3(-r, -r, +r), XMFLOAT4(0,1,0,1) };
	v[5] = { XMFLOAT3(-r, +r, +r), XMFLOAT4(0,1,0,1) };
	v[6] = { XMFLOAT3(+r, +r, +r), XMFLOAT4(0,1,0,1) };
	v[7] = { XMFLOAT3(+r, -r, +r), XMFLOAT4(0,1,0,1) };

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(PC) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = v;

	pDevice->CreateBuffer(&vbd, &vinitData, &mVB);

	UINT i[36];

	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	i[6] = 4; i[7] = 6; i[8] = 5;
	i[9] = 4; i[10] = 7; i[11] = 6;

	i[12] = 4; i[13] = 5; i[14] = 1;
	i[15] = 4; i[16] = 1; i[17] = 0;

	i[18] = 3; i[19] = 2; i[20] = 6;
	i[21] = 3; i[22] = 6; i[23] = 7;

	i[24] = 1; i[25] = 5; i[26] = 6;
	i[27] = 1; i[28] = 6; i[29] = 2;

	i[30] = 4; i[31] = 0; i[32] = 3;
	i[33] = 4; i[34] = 3; i[35] = 7;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = i;
	pDevice->CreateBuffer(&ibd, &iinitData, &mIB);

	return S_OK;
}

VOID Cube::Update()
{
	Object::Update();
}

void Cube::Draw(ID3D11DeviceContext *dc, CXMMATRIX ViewProj)
{
	dc->IASetInputLayout(Layout::mPC);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	LPD3D11EFFECTTECHNIQUE activeTech = Effects::PointEffectFX->mTech;

	UINT stride = sizeof(PC);
	UINT offset = 0;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMMATRIX M = XMMatrixMultiply(mWorld, ViewProj);

	Effects::PointEffectFX->SetWroldViewProj(M);

	dc->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	dc->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	for (UINT p = 0; p < techDesc.Passes; p++)
	{
		activeTech->GetPassByIndex(p)->Apply(0, dc);
		dc->DrawIndexed(36, 0, 0);
	}
}
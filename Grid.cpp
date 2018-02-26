#include "stdafx.h"
#include "Grid.h"

Grid::Grid()
	: mSize(0)
{
}

Grid::~Grid()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

HRESULT Grid::Init(ID3D11Device * pDevice, int size)
{
	mSize = size;

	auto totalSize = mSize * 2;
	auto totalV = totalSize * 2;
	auto cell = mSize / 2;

	PC *v = new PC[totalV + 6];
	int a = 0;
	for (int i = 0; i < totalV; i += 4)
	{
		if (cell - a == 0)
		{
			a++;
			continue;
		}
		v[i + 0] = { XMFLOAT3(cell - a, 0, cell), XMFLOAT4(1,1,1,1) };
		v[i + 1] = { XMFLOAT3(cell - a, 0, -cell), XMFLOAT4(1,1,1,1) };
		v[i + 2] = { XMFLOAT3(cell, 0, cell - a), XMFLOAT4(1,1,1,1) };
		v[i + 3] = { XMFLOAT3(-cell, 0, cell - a), XMFLOAT4(1,1,1,1) };
		a++;
	}

	v[totalV + 0] = { XMFLOAT3(0,0,cell), XMFLOAT4(0,0,1,1) };
	v[totalV + 1] = { XMFLOAT3(0,0,-cell), XMFLOAT4(0,0,1,1) };
	v[totalV + 2] = { XMFLOAT3(cell,0,0), XMFLOAT4(1,0,0,1) };
	v[totalV + 3] = { XMFLOAT3(-cell,0,0), XMFLOAT4(1,0,0,1) };
	v[totalV + 4] = { XMFLOAT3(0,cell,0), XMFLOAT4(0,1,0,1) };
	v[totalV + 5] = { XMFLOAT3(0,-cell,0), XMFLOAT4(0,1,0,1) };

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(PC) * totalV + sizeof(PC) * 6;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = v;
	pDevice->CreateBuffer(&vbd, &vinitData, &mVB);

	UINT *ii = new UINT[totalV + 6];
	for (int i = 0; i < totalV + 6; i++)
	{
		ii[i] = i;
	}
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalV + sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = ii;
	pDevice->CreateBuffer(&ibd, &iinitData, &mIB);

	return S_OK;
}

VOID Grid::Draw(ID3D11DeviceContext * dc, CXMMATRIX ViewProj)
{
	dc->IASetInputLayout(Layout::mPC);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	LPD3D11EFFECTTECHNIQUE activeTech = Effects::PointEffectFX->mTech;

	UINT stride = sizeof(PC);
	UINT offset = 0;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMMATRIX M = ViewProj;

	Effects::PointEffectFX->SetWorldViewProj(M);

	dc->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	dc->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	for (UINT p = 0; p < techDesc.Passes; p++)
	{
		activeTech->GetPassByIndex(p)->Apply(0, dc);
		dc->DrawIndexed(mSize * 4 + 6, 0, 0);
	}
}
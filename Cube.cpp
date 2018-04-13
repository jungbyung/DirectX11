#include "stdafx.h"
#include "Cube.h"

Cube::Cube()
{
}

Cube::~Cube()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	ReleaseCOM(mDiffuse);
}

HRESULT Cube::Init(ID3D11Device * pDevice)
{
	TexMetadata tmData;
	ScratchImage img;
	LoadFromDDSFile(L"resource/texture/WoodCrate01.dds", 0, &tmData, img);
	CreateShaderResourceView(pDevice, img.GetImages(), img.GetImageCount(), tmData, &mDiffuse);

	float r = 0.5f;

	PNT v[] =
	{
		{ XMFLOAT3(-r, -r, -r), XMFLOAT3(0,0,-1), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-r, +r, -r), XMFLOAT3(0,0,-1), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(+r, +r, -r), XMFLOAT3(0,0,-1), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(+r, -r, -r), XMFLOAT3(0,0,-1), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(-r, -r, +r), XMFLOAT3(0,0,1), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(+r, -r, +r), XMFLOAT3(0,0,1), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(+r, +r, +r), XMFLOAT3(0,0,1), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-r, +r, +r), XMFLOAT3(0,0,1), XMFLOAT2(1.0f, 0.0f) },

		{ XMFLOAT3(-r, +r, -r), XMFLOAT3(0,1,0), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-r, +r, +r), XMFLOAT3(0,1,0), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(+r, +r, +r), XMFLOAT3(0,1,0), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(+r, +r, -r), XMFLOAT3(0,1,0), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(-r, -r, -r), XMFLOAT3(0,-1,0), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(+r, -r, -r), XMFLOAT3(0,-1,0), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(+r, -r, +r), XMFLOAT3(0,-1,0), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-r, -r, +r), XMFLOAT3(0,-1,0), XMFLOAT2(1.0f, 0.0f) },

		{ XMFLOAT3(-r, -r, +r), XMFLOAT3(-1,0,0), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-r, +r, +r), XMFLOAT3(-1,0,0), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-r, +r, -r), XMFLOAT3(-1,0,0), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-r, -r, -r), XMFLOAT3(-1,0,0), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(+r, -r, -r), XMFLOAT3(1,0,0), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(+r, +r, -r), XMFLOAT3(1,0,0), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(+r, +r, +r), XMFLOAT3(1,0,0), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(+r, -r, +r), XMFLOAT3(1,0,0), XMFLOAT2(1.0f, 1.0f) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(PNT) * 24;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = v;

	pDevice->CreateBuffer(&vbd, &vinitData, &mVB);

	UINT i[] =
	{
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23
	};

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

VOID Cube::Update(float delta)
{
	Object::Update();
	Object::Moving(delta);
}

VOID Cube::Draw(ID3D11DeviceContext * dc, CXMMATRIX ViewProj)
{
	dc->IASetInputLayout(Layout::mPNT);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	LPD3D11EFFECTTECHNIQUE activeTech = Effects::BasicEffectFX->mTech;

	UINT stride = sizeof(PNT);
	UINT offset = 0;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMMATRIX M = XMMatrixMultiply(XMLoadFloat4x4(&mWorld), ViewProj);

	Effects::BasicEffectFX->SetWorldViewProj(M);
	Effects::BasicEffectFX->SetDiffuse(mDiffuse);

	dc->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	dc->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	for (UINT p = 0; p < techDesc.Passes; p++)
	{
		activeTech->GetPassByIndex(p)->Apply(0, dc);
		dc->DrawIndexed(36, 0, 0);
	}
}

ColorCube::ColorCube()
{
}

ColorCube::~ColorCube()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
}

HRESULT ColorCube::Init(ID3D11Device * pDevice)
{
	PC v[8];

	float r = 0.5f;

	v[0] = { XMFLOAT3(-r, -r, -r), XMFLOAT4(1,0,0,1) };
	v[1] = { XMFLOAT3(-r, +r, -r), XMFLOAT4(0,1,0,1) };
	v[2] = { XMFLOAT3(+r, +r, -r), XMFLOAT4(0,0,1,1) };
	v[3] = { XMFLOAT3(+r, -r, -r), XMFLOAT4(1,1,0,1) };
	v[4] = { XMFLOAT3(-r, -r, +r), XMFLOAT4(0,1,1,1) };
	v[5] = { XMFLOAT3(-r, +r, +r), XMFLOAT4(1,0,1,1) };
	v[6] = { XMFLOAT3(+r, +r, +r), XMFLOAT4(1,1,1,1) };
	v[7] = { XMFLOAT3(+r, -r, +r), XMFLOAT4(0,0,0,1) };

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

VOID ColorCube::Update(float delta)
{
	Object::Moving(delta);
	Object::Update();
}

VOID ColorCube::Draw(ID3D11DeviceContext *dc, CXMMATRIX ViewProj)
{
	dc->IASetInputLayout(Layout::mPC);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	LPD3D11EFFECTTECHNIQUE activeTech = Effects::PointEffectFX->mTech;

	UINT stride = sizeof(PC);
	UINT offset = 0;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMMATRIX M = XMMatrixMultiply(XMLoadFloat4x4(&mWorld), ViewProj);

	Effects::PointEffectFX->SetWorldViewProj(M);

	dc->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	dc->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	for (UINT p = 0; p < techDesc.Passes; p++)
	{
		activeTech->GetPassByIndex(p)->Apply(0, dc);
		dc->DrawIndexed(36, 0, 0);
	}
}

TextureCube::TextureCube()
{
}

TextureCube::~TextureCube()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	ReleaseCOM(mTexture);
}

HRESULT TextureCube::Init(ID3D11Device * pDevice)
{
	TexMetadata tmData;
	ScratchImage img;
	LoadFromDDSFile(L"resource/texture/WoodCrate01.dds", 0, &tmData, img);
	CreateShaderResourceView(pDevice, img.GetImages(), img.GetImageCount(), tmData, &mTexture);

	float r = 0.5f;

	PT v[] =
	{
		{XMFLOAT3(-r, -r, -r), XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(-r, +r, -r), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(+r, +r, -r), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(+r, -r, -r), XMFLOAT2(1.0f, 1.0f)},

		{XMFLOAT3(-r, -r, +r), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(+r, -r, +r), XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(+r, +r, +r), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-r, +r, +r), XMFLOAT2(1.0f, 0.0f)},

		{XMFLOAT3(-r, +r, -r), XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(-r, +r, +r), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(+r, +r, +r), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(+r, +r, -r), XMFLOAT2(1.0f, 1.0f)},

		{XMFLOAT3(-r, -r, -r), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(+r, -r, -r), XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(+r, -r, +r), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-r, -r, +r), XMFLOAT2(1.0f, 0.0f)},

		{XMFLOAT3(-r, -r, +r), XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(-r, +r, +r), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-r, +r, -r), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-r, -r, -r), XMFLOAT2(1.0f, 1.0f)},

		{XMFLOAT3(+r, -r, -r), XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(+r, +r, -r), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(+r, +r, +r), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(+r, -r, +r), XMFLOAT2(1.0f, 1.0f)}
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(PT) * 24;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = v;

	pDevice->CreateBuffer(&vbd, &vinitData, &mVB);

	UINT i[36];

	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

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

VOID TextureCube::Update(float delta)
{
	Object::Update();
	Object::Moving(delta);
}

VOID TextureCube::Draw(ID3D11DeviceContext * dc, CXMMATRIX ViewProj)
{
	dc->IASetInputLayout(Layout::mPT);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	LPD3D11EFFECTTECHNIQUE activeTech = Effects::TextureEffectFX->mTech;

	UINT stride = sizeof(PT);
	UINT offset = 0;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMMATRIX M = XMMatrixMultiply(XMLoadFloat4x4(&mWorld), ViewProj);

	Effects::TextureEffectFX->SetWorldViewProj(M);
	Effects::TextureEffectFX->SetTexture(mTexture);
	//Effects::BasicEffectFX->SetTexTransform(XMMatrixIdentity());

	dc->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	dc->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	for (UINT p = 0; p < techDesc.Passes; p++)
	{
		activeTech->GetPassByIndex(p)->Apply(0, dc);
		dc->DrawIndexed(36, 0, 0);
	}
}
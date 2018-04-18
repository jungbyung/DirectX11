#include "stdafx.h"
#include "Mesh.h"

void Subset::Initialize(ID3D11Device * pDevice)
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * mVertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &mVertices[0];
	pDevice->CreateBuffer(&vbd, &vinitData, &mVB);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mIndices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &mIndices[0];
	pDevice->CreateBuffer(&ibd, &iinitData, &mIB);

	_bstr_t d(mDiffuseTextureName.c_str());
	wchar_t* wd = d;
	DirectX::TexMetadata tmdata;
	DirectX::ScratchImage img;
	DirectX::LoadFromTGAFile(wd, &tmdata, img);
	DirectX::CreateShaderResourceView(pDevice, img.GetImages(), img.GetImageCount(), tmdata, &mDiffuseMapSRV);

	_bstr_t n(mNormalTextureName.c_str());
	wchar_t* wn = n;
	DirectX::LoadFromTGAFile(wn, &tmdata, img);
	DirectX::CreateShaderResourceView(pDevice, img.GetImages(), img.GetImageCount(), tmdata, &mNormalMapSRV);
}

void Subset::Update(const float deltaTime)
{
	Object::Update(deltaTime);
}

void Subset::Draw(ID3D11DeviceContext * dc)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	dc->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	dc->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	dc->DrawIndexed(mIndices.size(), 0, 0);
}

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	SafeDelete(mBone);
}

void Mesh::Initialize(ID3D11Device * pDevice)
{
	for (UINT i = 0; i < mSubset.size(); i++)
	{
		mSubset[i]->Initialize(pDevice);
	}
}

void Mesh::Update(const float deltaTime)
{
	Object::Update(deltaTime);
	for (UINT i = 0; i < mSubset.size(); i++)
	{
		mSubset[i]->Update(deltaTime);
	}
}

void Mesh::Draw(ID3D11DeviceContext * dc, CXMMATRIX viewproj)
{
	dc->IASetInputLayout(Layout::mPNTS);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	LPD3D11EFFECTTECHNIQUE activeTech = Effects::MeshEffectFX->mTech;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMMATRIX mat;
	mat = XMLoadFloat4x4(&mWorld);

	XMMATRIX worldInvTranspose = InverseTranspose(mat);
	XMMATRIX worldViewProj = mat * viewproj;

	Effects::MeshEffectFX->SetWorld(mat);
	Effects::MeshEffectFX->SetWorldInvTranspose(worldInvTranspose);
	Effects::MeshEffectFX->SetWorldViewProj(worldViewProj);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		for (UINT i = 0; i < mSubset.size(); ++i)
		{
			Effects::MeshEffectFX->SetDiffuseMap(mSubset[i]->GetMapSRV(0));
			Effects::MeshEffectFX->SetMaterial(mSubset[i]->GetMaterial());
			Effects::MeshEffectFX->SetNormalMap(mSubset[i]->GetMapSRV(2));

			activeTech->GetPassByIndex(p)->Apply(0, dc);
			mSubset[i]->Draw(dc);
		}
	}
}

StaticMesh::StaticMesh()
{
}

StaticMesh::~StaticMesh()
{
}
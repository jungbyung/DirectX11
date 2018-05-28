#include "stdafx.h"
#include "Util.h"
#include "Mesh.h"

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
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
const D3D11_INPUT_ELEMENT_DESC LayoutDesc::pnt[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
const D3D11_INPUT_ELEMENT_DESC LayoutDesc::pnts[6] =
{
	{ "POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "WEIGHTS",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEINDICES",  0, DXGI_FORMAT_R8G8B8A8_UINT,   0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

ID3D11InputLayout* Layout::mPC = nullptr;
ID3D11InputLayout* Layout::mPT = nullptr;
ID3D11InputLayout* Layout::mPNT = nullptr;
ID3D11InputLayout* Layout::mPNTS = nullptr;

Layout::Layout()
{
}

Layout::~Layout()
{
	ReleaseCOM(mPC);
	ReleaseCOM(mPT);
	ReleaseCOM(mPNT);
	ReleaseCOM(mPNTS);
}

VOID Layout::Init(ID3D11Device * pDevice)
{
	D3DX11_PASS_DESC passDesc;

	Effects::MeshEffectFX->mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HRESULT hr = pDevice->CreateInputLayout(LayoutDesc::pnts, 6, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mPNTS);

	Effects::PointEffectFX->mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	pDevice->CreateInputLayout(LayoutDesc::pc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mPC);

	Effects::BasicEffectFX->mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	pDevice->CreateInputLayout(LayoutDesc::pnt, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mPNT);

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

	Effects::PointEffectFX->SetWorldViewProj(M);

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

XMMATRIX JB::InverseTranspose(CXMMATRIX M)
{
	XMMATRIX A = M;
	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	XMVECTOR det = XMMatrixDeterminant(A);
	return XMMatrixTranspose(XMMatrixInverse(&det, A));
}

void JB::FindStringToBone(const string & JointName, IN Bone* bone, OUT Bone** b)
{
	if (bone->FindStringOfBone(JointName))
	{
		*b = bone;
		return;
	}
	for (UINT i = 0; i < bone->GetChildNum(); i++)
	{
		if (*b != nullptr) return;
		FindStringToBone(JointName, bone->GetChild(i), b);
	}
}

void JB::AddVectorTransform(Bone * bone, vector<XMFLOAT4X4>& m)
{
	m.push_back(bone->GetMatrix());
	for (int i = 0; i < bone->GetChildNum(); i++)
	{
		AddVectorTransform(bone->GetChild(i), m);
	}
}

void JB::OutBoneName(FILE** fp, Bone * bone)
{
	fprintf((*fp), "%s %d\n", bone->GetName().c_str(), bone->GetIndex());

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			fprintf(*fp, "%f ", bone->GetMatrix().m[i][j]);
		}
		fprintf(*fp, "\n");
	}

	for (int i = 0; i < bone->GetChildNum(); i++)
	{
		OutBoneName(&(*fp), bone->GetChild(i));
	}
}
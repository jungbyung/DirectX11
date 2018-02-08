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

ID3D11InputLayout* Layout::mPC = nullptr;

Layout::Layout()
{
}

Layout::~Layout()
{
	ReleaseCOM(mPC);
}

VOID Layout::Init(ID3D11Device * pDevice)
{
	D3DX11_PASS_DESC passDesc;
	Effects::PointEffectFX->mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	pDevice->CreateInputLayout(LayoutDesc::pc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mPC);
}
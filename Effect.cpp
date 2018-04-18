#include "stdafx.h"
#include "Effect.h"

Effect::Effect(ID3D11Device * pDevice, const char * fileName)
{
	FILE* fp;
	fopen_s(&fp, fileName, "rb");

	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	vector<char> compiledShader(size);

	fread(&compiledShader[0], sizeof(char), size, fp);
	fclose(fp);

	D3DX11CreateEffectFromMemory(&compiledShader[0], size, 0, pDevice, &mFX);
}

Effect::~Effect()
{
	ReleaseCOM(mFX);
}

PointEffect::PointEffect(ID3D11Device * pDevice, const char * fileName)
	: Effect(pDevice, fileName)
{
	mWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	mTech = mFX->GetTechniqueByName("ColorTech");
}

PointEffect::~PointEffect()
{
}

PointEffect*			Effects::PointEffectFX = nullptr;
TextureEffect*			Effects::TextureEffectFX = nullptr;
BasicEffect*			Effects::BasicEffectFX = nullptr;
MeshEffect*				Effects::MeshEffectFX = nullptr;

VOID Effects::Init(ID3D11Device * pDevice)
{
	PointEffectFX = new PointEffect(pDevice, "fx/PointColor.fxo");
	TextureEffectFX = new TextureEffect(pDevice, "fx/Texture.fxo");
	BasicEffectFX = new BasicEffect(pDevice, "fx/Basic.fxo");
	MeshEffectFX = new MeshEffect(pDevice, "fx/Mesh.fxo");
}

TextureEffect::TextureEffect(ID3D11Device * pDevice, const char * fileName)
	: Effect(pDevice, fileName)
{
	mWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	mTexTransform = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	mTech = mFX->GetTechniqueByName("TextureTech");
	mTex = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

TextureEffect::~TextureEffect()
{
}

BasicEffect::BasicEffect(ID3D11Device * pDevice, const char* fileName)
	: Effect(pDevice, fileName)
{
	mWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	mTech = mFX->GetTechniqueByName("BasicTech");
	mDiffuse = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

BasicEffect::~BasicEffect()
{
}

MeshEffect::MeshEffect(ID3D11Device * pDevice, const char * fileName)
	: Effect(pDevice, fileName)
{
	mTech = mFX->GetTechniqueByName("v");

	World = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	BoneTransforms = mFX->GetVariableByName("gBoneTransforms")->AsMatrix();
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	DiffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	NormalMap = mFX->GetVariableByName("gNormalMap")->AsShaderResource();
	EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
	DirLights = mFX->GetVariableByName("gDirLights");
	Mat = mFX->GetVariableByName("gMaterial");
}

MeshEffect::~MeshEffect()
{
}
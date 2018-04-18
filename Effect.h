#pragma once
#include "Util.h"
using JB::Material;
using JB::DirectionalLight;

class Effect
{
public:
	Effect(ID3D11Device* pDevice, const char* fileName);
	virtual ~Effect();
private:
	Effect(const Effect& rhs);
	Effect& operator=(const Effect&rhs);
protected:
	LPD3D11EFFECT mFX;
};

class PointEffect : public Effect
{
public:
	PointEffect(ID3D11Device* pDevice, const char* fileName);
	~PointEffect();

	ID3DX11EffectTechnique* mTech;

	ID3DX11EffectMatrixVariable* mWorldViewProj;

	VOID SetWorldViewProj(CXMMATRIX M) { mWorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
};

class TextureEffect : public Effect
{
public:
	TextureEffect(ID3D11Device* pDevice, const char* fileName);
	~TextureEffect();

	ID3DX11EffectTechnique* mTech;

	ID3DX11EffectMatrixVariable* mWorldViewProj;
	ID3DX11EffectShaderResourceVariable* mTex;
	ID3DX11EffectMatrixVariable* mTexTransform;

	VOID SetTexTransform(CXMMATRIX M) { mTexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	VOID SetWorldViewProj(CXMMATRIX M) { mWorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	VOID SetTexture(ID3D11ShaderResourceView* tex) { mTex->SetResource(tex); }
};

class BasicEffect : public Effect
{
public:
	BasicEffect(ID3D11Device * pDevice, const char* fileName);
	~BasicEffect();

	ID3DX11EffectTechnique* mTech;
	ID3DX11EffectMatrixVariable* mWorldViewProj;
	ID3DX11EffectShaderResourceVariable* mDiffuse;

	VOID SetWorldViewProj(CXMMATRIX M) { mWorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	VOID SetDiffuse(ID3D11ShaderResourceView* diffuse) { mDiffuse->SetResource(diffuse); }
};

class MeshEffect : public Effect
{
public:
	MeshEffect(ID3D11Device * pDevice, const char* fileName);
	~MeshEffect();

	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProjTex(CXMMATRIX M) { WorldViewProjTex->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
	void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex) { NormalMap->SetResource(tex); }
	void SetBoneTransforms(const XMFLOAT4X4* M, int cnt) { BoneTransforms->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }

	ID3DX11EffectTechnique* mTech;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProjTex;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectMatrixVariable* BoneTransforms;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* Mat;
	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
};

class Effects
{
public:
	static VOID Init(ID3D11Device* pDevice);

	static PointEffect* PointEffectFX;
	static TextureEffect* TextureEffectFX;
	static BasicEffect* BasicEffectFX;
	static MeshEffect* MeshEffectFX;
};
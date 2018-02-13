#pragma once

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

	VOID SetWroldViewProj(CXMMATRIX M) { mWorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
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
	VOID SetWroldViewProj(CXMMATRIX M) { mWorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	VOID SetTexture(ID3D11ShaderResourceView* tex) { mTex->SetResource(tex); }
};

class Effects
{
public:
	static VOID Init(ID3D11Device* pDevice);

	static PointEffect* PointEffectFX;
	static TextureEffect* TextureEffectFX;
};
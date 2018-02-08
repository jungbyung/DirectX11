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

class Effects
{
public:
	static VOID Init(ID3D11Device* pDevice);

	static PointEffect* PointEffectFX;
};
#pragma once
#include "Object.h"

class Cube : public Object
{
private:
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	ID3D11ShaderResourceView* mDiffuse;
public:
	Cube();
	~Cube();

	HRESULT Init(ID3D11Device* pDevice);
	VOID Update(float delta = 0.0f);
	VOID Draw(ID3D11DeviceContext* dc, CXMMATRIX ViewProj);
};

class ColorCube : public Object
{
private:
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	struct PC
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
	};
	//ID3D11ShaderResourceView*
public:
	ColorCube();
	~ColorCube();

	HRESULT Init(ID3D11Device* pDevice);
	VOID Update(float delta = 0.0f);
	VOID Draw(ID3D11DeviceContext* dc, CXMMATRIX ViewProj);
};

class TextureCube : public Object
{
private:
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;
	struct PT
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};

	ID3D11ShaderResourceView* mTexture;

public:
	TextureCube();
	~TextureCube();

	HRESULT Init(ID3D11Device* pDevice);
	VOID Update(float delta = 0.0f);
	VOID Draw(ID3D11DeviceContext* dc, CXMMATRIX ViewProj);
};

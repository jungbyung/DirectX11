#pragma once
#include "Object.h"

class Cube : public Object
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
	Cube();
	~Cube();

	HRESULT Init(ID3D11Device* pDevice);
	VOID Update();
	void Draw(ID3D11DeviceContext* dc, CXMMATRIX ViewProj);
};

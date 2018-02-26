#pragma once

class Grid
{
private:
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	int mSize;
public:
	Grid();
	~Grid();

	HRESULT Init(ID3D11Device* pDevice, int size);
	VOID Draw(ID3D11DeviceContext* dc, CXMMATRIX ViewProj);
};

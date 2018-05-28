#pragma once

class Plance
{
private:
	vector<JB::PC> mVertices;
	vector<UINT> mIndices;

public:
	Plance();
	~Plance();

	void Init(ID3D11Device* pDevice, int n, int m);
	void Draw(ID3D11DeviceContext* dc, XMMATRIX mat);
};

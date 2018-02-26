#pragma once

#include "resource.h"
#include "App.h"

class Cube;
class TextureCube;
class Camera;
class Grid;

class DirectX11 : public App
{
private:
	Grid* mGrid;
	Cube* mCube;
	Camera* mCamera;

	TextureCube* mTCube;
public:
	DirectX11(HINSTANCE hInstance);
	~DirectX11();

	virtual HRESULT Init();
	virtual VOID OnResize();
	virtual VOID UpdateScene(float dt);
	virtual VOID DrawScene();
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
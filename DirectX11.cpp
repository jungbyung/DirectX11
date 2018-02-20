// FBXConvert.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DirectX11.h"
#include "Cube.h"
#include "Camera.h"
#include "Effect.h"
#include "Util.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	DirectX11 app(hInstance);

	if (FAILED(app.Init()))
		return 0;

	return app.Run();
}

DirectX11::DirectX11(HINSTANCE hInstance)
	: App(hInstance)

{
}

DirectX11::~DirectX11()
{
}

HRESULT DirectX11::Init()
{
	App::Init();
	Effects::Init(mDevice);
	Layout::Init(mDevice);
	mCube = new Cube;
	if (FAILED(mCube->Init(mDevice)))
		return E_FAIL;

	mTCube = new TextureCube;
	if (FAILED(mTCube->Init(mDevice)))
		return E_FAIL;

	mCamera = new Camera;
	mCamera->SetLens(0.25*XM_PI, (static_cast<float>(mClientWidth) / mClientHeight), 1.0f, 1000.0f);
	mCamera->SetPosition(0, 0, 10);

	return S_OK;
}

VOID DirectX11::OnResize()
{
	App::OnResize();
}

VOID DirectX11::UpdateScene(float dt)
{
	mCube->Update();
	mTCube->Update(dt);
	mCamera->Update();
}

VOID DirectX11::DrawScene()
{
	float color[] = { 0.32f, 0.32f, 0.32f, 1.0f };
	mImmediateContext->ClearRenderTargetView(mRenderTargetView, color);
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	Line(mDevice, mImmediateContext, XMFLOAT3(-2, 0, 0), XMFLOAT3(2, 0, 0), XMFLOAT4(1, 1, 1, 1), mCamera->GetViewProj());
	//	mCube->Draw(mImmediateContext, mCamera->GetViewProj());
	mTCube->Draw(mImmediateContext, mCamera->GetViewProj());

	mSwapChain->Present(0, 0);
}

LRESULT DirectX11::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return App::MsgProc(hwnd, msg, wParam, lParam);
}
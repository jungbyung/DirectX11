// FBXConvert.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DirectX11.h"
#include "Cube.h"
#include "Camera.h"
#include "Effect.h"
#include "Util.h"
#include "Grid.h"

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

	mGrid = new Grid;
	mGrid->Init(mDevice, 11);

	mCube = new Cube;
	if (FAILED(mCube->Init(mDevice)))
		return E_FAIL;

	//mTCube = new TextureCube;
	//if (FAILED(mTCube->Init(mDevice)))
	//	return E_FAIL;

	//mTCube->SetPosition(1, 0, 0);

	mCamera = new Camera;
	mCamera->SetLens(0.25*XM_PI, (static_cast<float>(mClientWidth) / mClientHeight), 1.0f, 1000.0f);
	mCamera->SetPosition(0, 5, -10);
	mCamera->LookAt(mCamera->GetFloat3Pos(), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1, 0));
	printf("%f, %f, %f\n", mCamera->GetFloat3Pos().x, mCamera->GetFloat3Pos().y, mCamera->GetFloat3Pos().z);
	printf("%f, %f, %f\n", mCamera->GetFloat3Forward().x, mCamera->GetFloat3Forward().y, mCamera->GetFloat3Forward().z);
	return S_OK;
}

VOID DirectX11::OnResize()
{
	App::OnResize();
}

VOID DirectX11::UpdateScene(float dt)
{
	mCube->Update(dt);
	//mTCube->Update(dt);
	mCamera->LookAt(mCamera->GetFloat3Pos(), mCube->GetPosition(), XMFLOAT3(0, 1, 0));
	mCamera->Update();
}

VOID DirectX11::DrawScene()
{
	float color[] = { 0.32f, 0.32f, 0.32f, 1.0f };
	mImmediateContext->ClearRenderTargetView(mRenderTargetView, color);
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	mGrid->Draw(mImmediateContext, mCamera->GetViewProj());
	//Line(mDevice, mImmediateContext, XMFLOAT3(-2, 0, 0), XMFLOAT3(2, 0, 0), XMFLOAT4(1, 1, 1, 1), mCamera->GetViewProj());
	mCube->Draw(mImmediateContext, mCamera->GetViewProj());
	//mTCube->Draw(mImmediateContext, mCamera->GetViewProj());

	mSwapChain->Present(0, 0);
}

LRESULT DirectX11::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return App::MsgProc(hwnd, msg, wParam, lParam);
}
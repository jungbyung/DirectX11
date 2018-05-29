// FBXConvert.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DirectX11.h"
#include "Cube.h"
#include "Camera.h"
#include "Effect.h"
#include "Util.h"
#include "Grid.h"
#include "FBXLoader.h"
#include "Mesh.h"

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

	mCCube = new ColorCube;
	mCCube->Init(mDevice);

	//mTCube = new TextureCube;
	//if (FAILED(mTCube->Init(mDevice)))
	//	return E_FAIL;

	//mTCube->SetPosition(1, 0, 0);

	mMesh = LOADFBX->LoadFBX("./resource/model/model.fbx");
	mMesh->Initialize(mDevice);

	XMVECTOR q;

	//q = XMQuaternionRotationRollPitchYaw(90, 90, 0);

	q = XMVectorSet(XM_PI / 2.f, XM_PI / 2.0f, 0, 0);
	mMesh->SetRotation(q);
	mMesh->SetScale(XMVectorSet(0.1f, 0.1f, 0.1f, 0));

	mDirLight.Direction = XMFLOAT3(0, 0, 1);

	mDirLight.mAmbient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLight.mDiffuse = XMFLOAT4(1, 1, 1, 1.f);
	mDirLight.mSpecular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.f);

	mCamera = new Camera;
	mCamera->SetLens(0.25*XM_PI, (static_cast<float>(mClientWidth) / mClientHeight), 1.0f, 1000.0f);
	mCamera->SetPosition(XMVectorSet(0, 5, -20, 0));
	mCamera->LookAt(XMLoadFloat3(&mCamera->GetPosition()), XMLoadFloat3(&mMesh->GetPosition()));

	return S_OK;
}

VOID DirectX11::OnResize()
{
	App::OnResize();
}

VOID DirectX11::UpdateScene(float dt)
{
	static bool s = false;
	mCube->Update(dt);
	mCCube->Update(dt);
	//mTCube->Update(dt);
	mCamera->LookAt(XMLoadFloat3(&mCamera->GetPosition()), XMLoadFloat3(&mMesh->GetPosition()));
	mCamera->Update();

	mMesh->Update(dt);

	if (GetAsyncKeyState(VK_F1) & 0x8001)
	{
		mSwapChain->SetFullscreenState(s, nullptr);
		s = !s;
	}
}

VOID DirectX11::DrawScene()
{
	float color[] = { 0.32f, 0.32f, 0.32f, 1.0f };
	mImmediateContext->ClearRenderTargetView(mRenderTargetView, color);
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	XMMATRIX ViewProj = XMMatrixMultiply(XMLoadFloat4x4(&mCamera->GetView()), XMLoadFloat4x4(&mCamera->GetProj()));

	Effects::MeshEffectFX->SetDirLights(&mDirLight);

	mGrid->Draw(mImmediateContext, ViewProj);
	//Line(mDevice, mImmediateContext, XMFLOAT3(-2, 0, 0), XMFLOAT3(2, 0, 0), XMFLOAT4(1, 1, 1, 1), mCamera->GetViewProj());
	//mCube->Draw(mImmediateContext, mCamera->GetViewProj());
	//mCCube->Draw(mImmediateContext, ViewProj);
	//mTCube->Draw(mImmediateContext, mCamera->GetViewProj());

	mMesh->Draw(mImmediateContext, ViewProj);

	mSwapChain->Present(0, 0);
}

LRESULT DirectX11::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return App::MsgProc(hwnd, msg, wParam, lParam);
}
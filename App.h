#pragma once
#include "Timer.h"

class App
{
protected:
	ID3D11Device*				mDevice;
	ID3D11DeviceContext*		mImmediateContext;
	IDXGISwapChain*				mSwapChain;
	ID3D11RenderTargetView*		mRenderTargetView;
	ID3D11DepthStencilView*		mDepthStencilView;
	ID3D11Texture2D*			mDepthStencilBuffer;
	D3D11_VIEWPORT				mScreenViewport;

	BOOL						mEnable4xMsaa;
	UINT						m4xMsaaQuality;

	Timer						mTimer;

	BOOL						mAppPaused;
	BOOL						mMinimized;
	BOOL						mMaximized;
	BOOL						mResizing;

	HINSTANCE					mhAppInst;
	HWND						mhMainWnd;

	string						mMainWndCaption;
	D3D_DRIVER_TYPE				mDriverType;
	INT							mClientWidth;
	INT							mClientHeight;
public:
	App(HINSTANCE hInstance);
	virtual ~App();

	INT Run();
	virtual HRESULT Init();
	virtual VOID OnResize();
	virtual VOID UpdateScene(float dt) = 0;
	virtual VOID DrawScene() = 0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HINSTANCE GetAppInst() const;
	HWND GetMainWnd() const;
	float GetAspect() const;

	VOID CalculateFrameState();
	VOID OnConsole();
protected:

	HRESULT InitMainWindow();
	HRESULT InitDirect3D();
};

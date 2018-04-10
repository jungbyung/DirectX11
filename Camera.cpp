#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
	: mPos(0, 0, 10)
	, mRight(1, 0, 0)
	, mForward(0, 0, -1)
	, mUp(0, 1, 0)
	, mWorld(XMMatrixIdentity())
{
}

Camera::~Camera()
{
}

void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);

	mProj = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
}

void Camera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMVECTOR F = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, F));
	XMVECTOR U = XMVector3Cross(F, R);

	XMStoreFloat3(&mPos, pos);
	XMStoreFloat3(&mForward, F);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
}

void Camera::LookAt(const XMFLOAT3 & pos, const XMFLOAT3 & target, const XMFLOAT3 up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);
}

void Camera::SetPosition(float x, float y, float z)
{
	mPos = XMFLOAT3(x, y, z);
}

void Camera::SetPosition(const XMFLOAT3 & v)
{
	mPos = v;
}

void Camera::Strafe(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPos);

	XMStoreFloat3(&mPos, XMVectorMultiplyAdd(s, r, p));
}

void Camera::Walk(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR f = XMLoadFloat3(&mForward);
	XMVECTOR p = XMLoadFloat3(&mPos);

	XMStoreFloat3(&mPos, XMVectorMultiplyAdd(s, f, p));
}

void Camera::Pitch(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mForward, XMVector3TransformNormal(XMLoadFloat3(&mForward), R));
}

void Camera::RotationY(float angle)
{
	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mForward, XMVector3TransformNormal(XMLoadFloat3(&mForward), R));
}

void Camera::Update()
{
	XMVECTOR R = XMLoadFloat3(&mRight);
	XMVECTOR U = XMLoadFloat3(&mUp);
	XMVECTOR F = XMLoadFloat3(&mForward);
	XMVECTOR P = XMLoadFloat3(&mPos);

	F = XMVector3Normalize(F);
	U = XMVector3Normalize(XMVector3Cross(F, R));
	R = XMVector3Cross(U, F);

	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, F));

	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
	XMStoreFloat3(&mForward, F);

	mView._11 = mRight.x;
	mView._21 = mRight.y;
	mView._31 = mRight.z;
	mView._41 = x;

	mView._12 = mUp.x;
	mView._22 = mUp.y;
	mView._32 = mUp.z;
	mView._42 = y;

	mView._13 = mForward.x;
	mView._23 = mForward.y;
	mView._33 = mForward.z;
	mView._43 = z;

	mView._14 = 0.0f;
	mView._24 = 0.0f;
	mView._34 = 0.0f;
	mView._44 = 1.0f;
}

VOID Camera::Moving(float delta)
{
	if (GetAsyncKeyState('W') & 0x8000)
		Walk(delta);
	if (GetAsyncKeyState('S') & 0x8000)
		Walk(-delta);
	if (GetAsyncKeyState('A') & 0x8000)
		Strafe(-delta);
	if (GetAsyncKeyState('D') & 0x8000)
		Strafe(delta);

	//if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	//	RotationY(-delta);
	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	//	RotationY(delta);
	//if (GetAsyncKeyState(VK_UP) & 0x8000)
	//	Pitch(delta);
	//if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//	Pitch(-delta);
}
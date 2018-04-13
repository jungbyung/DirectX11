#include "stdafx.h"
#include "Object.h"

Object::Object()
	: mPosition(0, 0, 0)
	, mScale(1, 1, 1)
	, mForward(0, 0, 1)
	, mUp(0, 1, 0)
	, mRight(1, 0, 0)
	, mParent(nullptr)
{
	XMStoreFloat4x4(&mLocal, XMMatrixIdentity());
	XMStoreFloat4x4(&mWorld, XMMatrixIdentity());
}

Object::~Object()
{
	vector<Object*>::iterator siter = mChilds.begin();
	vector<Object*>::iterator eiter = mChilds.end();
	for (; siter != eiter; ++siter)
	{
		SafeDelete(*siter);
	}
	mChilds.clear();
	SafeDelete(mParent);
}

void Object::Update()
{
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR u = XMLoadFloat3(&mUp);
	XMVECTOR f = XMLoadFloat3(&mForward);
	XMVECTOR s = XMLoadFloat3(&mScale);

	r = XMVectorMultiply(r, s);
	u = XMVectorMultiply(u, s);
	f = XMVectorMultiply(f, s);

	memcpy(&mLocal.m[0], &r, sizeof(XMFLOAT3));
	memcpy(&mLocal.m[1], &u, sizeof(XMFLOAT3));
	memcpy(&mLocal.m[2], &f, sizeof(XMFLOAT3));
	memcpy(&mLocal.m[3], &mPosition, sizeof(XMFLOAT3));

	if (mParent)
	{
		XMStoreFloat4x4(&mWorld, XMMatrixMultiply(XMLoadFloat4x4(&mLocal), XMLoadFloat4x4(&mParent->GetWorldMatrix())));
	}
	else
	{
		mWorld = mLocal;
	}
}

void Object::SetPosition(FXMVECTOR pos)
{
	XMStoreFloat3(&mPosition, pos);
}

void Object::MovePosition(FXMVECTOR delta)
{
	XMVECTOR r = XMVector3Normalize(XMLoadFloat3(&mRight));
	XMVECTOR u = XMVector3Normalize(XMLoadFloat3(&mUp));
	XMVECTOR f = XMVector3Normalize(XMLoadFloat3(&mForward));

	XMVECTOR m = XMVectorSet(0, 0, 0, 0);

	XMVECTOR p = XMLoadFloat3(&mPosition);

	XMFLOAT3 d;
	XMStoreFloat3(&d, delta);

	m += r * d.x;
	m += u * d.x;
	m += f * d.x;

	p += m;

	XMStoreFloat3(&mPosition, p);
}

void Object::SetScale(FXMVECTOR scale)
{
	XMStoreFloat3(&mScale, scale);
}

void Object::SetRotation(FXMVECTOR rot)
{
	XMFLOAT3 Rot;

	XMStoreFloat3(&Rot, rot);

	XMMATRIX matX = XMMatrixRotationX(Rot.x);
	XMMATRIX matY = XMMatrixRotationY(Rot.y);
	XMMATRIX matZ = XMMatrixRotationZ(Rot.z);

	XMMATRIX XYZ = matX * matY * matZ;

	XMVECTOR r = XMVector3Normalize(XMLoadFloat3(&mRight));
	XMVECTOR u = XMVector3Normalize(XMLoadFloat3(&mUp));
	XMVECTOR f = XMVector3Normalize(XMLoadFloat3(&mForward));

	r = XMVector3TransformNormal(r, XYZ);
	u = XMVector3TransformNormal(u, XYZ);
	f = XMVector3TransformNormal(f, XYZ);

	XMStoreFloat3(&mRight, r);
	XMStoreFloat3(&mUp, u);
	XMStoreFloat3(&mForward, f);
}

void Object::LookDirection(FXMVECTOR dir, FXMVECTOR up)
{
	XMVECTOR newUp;
	XMVECTOR newRight;
	newRight = XMVector3Normalize(XMVector3Cross(up, dir));

	newUp = XMVector3Normalize(XMVector3Cross(dir, newRight));

	XMStoreFloat3(&mForward, dir);
	XMStoreFloat3(&mUp, newUp);
	XMStoreFloat3(&mRight, newRight);
}

void Object::LookPosition(FXMVECTOR pos, FXMVECTOR up)
{
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMVECTOR dir = XMVector3Normalize(pos - p);

	LookDirection(dir, up);
}

void Object::Walk(float delta)
{
	XMVECTOR s = XMVectorReplicate(delta * 10);
	XMVECTOR l = XMLoadFloat3(&mForward);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));
}

void Object::Strafe(float delta)
{
	XMVECTOR s = XMVectorReplicate(delta * 10);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));
}

void Object::Pitch(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mForward, XMVector3TransformNormal(XMLoadFloat3(&mForward), R));
}

void Object::RotationY(float angle)
{
	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mForward, XMVector3TransformNormal(XMLoadFloat3(&mForward), R));
}

VOID Object::MouseMove(POINT pt)
{
	float dx = -XMConvertToRadians(0.25f * pt.x);
	//float dy = -XMConvertToRadians(0.25f * pt.y);

	//Pitch(dy);
	RotationY(dx);
}

VOID Object::Moving(float delta)
{
	if (GetAsyncKeyState('W') & 0x8000)
		Walk(delta);
	if (GetAsyncKeyState('S') & 0x8000)
		Walk(-delta);
	if (GetAsyncKeyState('A') & 0x8000)
		Strafe(-delta);
	if (GetAsyncKeyState('D') & 0x8000)
		Strafe(delta);

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		RotationY(-delta);
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		RotationY(delta);
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		Pitch(delta);
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		Pitch(-delta);
}
#include "stdafx.h"
#include "Object.h"

Object::Object()
	: mPosition(0, 0, 0)
	, mScale(1, 1, 1)
	, mForward(0, 0, 1)
	, mUp(0, 1, 0)
	, mRight(1, 0, 0)
	, mLocal(XMMatrixIdentity())
	, mWorld(XMMatrixIdentity())
	, mParent(nullptr)
{
}

Object::~Object()
{
}

void Object::Update()
{
	mLocal = XMMatrixIdentity();

	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR u = XMLoadFloat3(&mUp);
	XMVECTOR f = XMLoadFloat3(&mForward);
	XMVECTOR s = XMLoadFloat3(&mScale);

	r = XMVectorMultiply(r, s);
	u = XMVectorMultiply(u, s);
	f = XMVectorMultiply(f, s);

	memcpy(&mLocal.r[0], &r, sizeof(XMFLOAT3));
	memcpy(&mLocal.r[1], &u, sizeof(XMFLOAT3));
	memcpy(&mLocal.r[2], &f, sizeof(XMFLOAT3));
	memcpy(&mLocal.r[3], &mPosition, sizeof(XMFLOAT3));

	if (mParent)
	{
		mWorld = XMMatrixMultiply(mLocal, mParent->GetWorldMatrix());
	}
	else
	{
		mWorld = mLocal;
	}
}

void Object::SetPosition(float x, float y, float z)
{
	mPosition = XMFLOAT3(x, y, z);
}

void Object::SetPosition(const XMFLOAT3 & pos)
{
	mPosition = pos;
}

void Object::MovePosition(float x, float y, float z)
{
	XMVECTOR r = XMVector3Normalize(XMLoadFloat3(&mRight));
	XMVECTOR u = XMVector3Normalize(XMLoadFloat3(&mUp));
	XMVECTOR f = XMVector3Normalize(XMLoadFloat3(&mForward));

	XMVECTOR m = XMVectorSet(0, 0, 0, 0);

	XMVECTOR p = XMLoadFloat3(&mPosition);

	m += r * x;
	m += u * y;
	m += f * z;

	p += m;

	XMStoreFloat3(&mPosition, p);
}

void Object::MovePosition(const XMFLOAT3 & delta)
{
	XMVECTOR r = XMVector3Normalize(XMLoadFloat3(&mRight));
	XMVECTOR u = XMVector3Normalize(XMLoadFloat3(&mUp));
	XMVECTOR f = XMVector3Normalize(XMLoadFloat3(&mForward));

	XMVECTOR m = XMVectorSet(0, 0, 0, 0);

	XMVECTOR p = XMLoadFloat3(&mPosition);

	m += r * delta.x;
	m += u * delta.y;
	m += f * delta.z;

	p += m;

	XMStoreFloat3(&mPosition, p);
}

void Object::SetScale(float x, float y, float z)
{
	mScale = XMFLOAT3(x, y, z);
}

void Object::SetScale(const XMFLOAT3 & scale)
{
	mScale = scale;
}

void Object::SetRotation(float x, float y, float z)
{
	XMMATRIX matX = XMMatrixRotationX(x);
	XMMATRIX matY = XMMatrixRotationY(y);
	XMMATRIX matZ = XMMatrixRotationZ(z);

	XMMATRIX rot = matX * matY * matZ;

	XMVECTOR r = XMVector3Normalize(XMLoadFloat3(&mRight));
	XMVECTOR u = XMVector3Normalize(XMLoadFloat3(&mUp));
	XMVECTOR f = XMVector3Normalize(XMLoadFloat3(&mForward));

	r = XMVector3TransformNormal(r, rot);
	u = XMVector3TransformNormal(u, rot);
	f = XMVector3TransformNormal(f, rot);

	XMStoreFloat3(&mRight, r);
	XMStoreFloat3(&mUp, u);
	XMStoreFloat3(&mForward, f);
}

void Object::SetRotation(const XMFLOAT3 & rot)
{
	SetRotation(rot.x, rot.y, rot.z);
}

void Object::LookDirection(const XMFLOAT3 & dir, const XMFLOAT3 & up)
{
	XMVECTOR newForward = XMLoadFloat3(&dir);
	XMVECTOR newUp = XMLoadFloat3(&up);
	XMVECTOR newRight;
	newRight = XMVector3Normalize(XMVector3Cross(newUp, newForward));

	newUp = XMVector3Normalize(XMVector3Cross(newForward, newRight));

	XMStoreFloat3(&mForward, newForward);
	XMStoreFloat3(&mUp, newUp);
	XMStoreFloat3(&mRight, newRight);
}

void Object::LookPosition(const XMFLOAT3 & pos, const XMFLOAT3 & up)
{
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMVECTOR dir = XMVector3Normalize(XMLoadFloat3(&pos) - p);

	XMFLOAT3 d;

	XMStoreFloat3(&d, dir);
	LookDirection(d, up);
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
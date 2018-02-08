#pragma once
#include "Object.h"

class Camera
{
private:
	XMFLOAT3 mPos;
	XMFLOAT3 mForward;
	XMFLOAT3 mUp;
	XMFLOAT3 mRight;

	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;

	XMMATRIX mView;
	XMMATRIX mProj;

	XMMATRIX mWorld;
	XMMATRIX mLocal;

public:
	Camera();
	~Camera();

	void SetLens(float fovY, float aspect, float zn, float zf);
	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3 up);

	XMMATRIX GetView() const { return mView; }
	XMMATRIX GetProj() const { return mProj; }
	XMMATRIX GetViewProj() const { return XMMatrixMultiply(mView, mProj); }

	XMVECTOR GetVectorPos()const { return XMLoadFloat3(&mPos); }
	XMFLOAT3 GetFloat3Pos()const { return mPos; }

	XMVECTOR GetVectorForward() const { return XMLoadFloat3(&mForward); }
	XMFLOAT3 GetFloat3Forward() const { return mForward; }

	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& v);

	void Strafe(float d);
	void Walk(float d);

	void Pitch(float angle);
	void RotationY(float angle);

	void Update();
};

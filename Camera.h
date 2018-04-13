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

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mLocal;

public:
	Camera();
	~Camera();

	void SetLens(float fovY, float aspect, float zn, float zf);
	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp = XMVectorSet(0, 1, 0, 1));

	const XMFLOAT4X4& GetView() { return mView; }
	const XMFLOAT4X4& GetProj() { return mProj; }

	const XMFLOAT3& GetPosition() { return mPos; }
	const XMFLOAT3& GetForward() { return mForward; }

	void SetPosition(FXMVECTOR pos);

	void Strafe(float d);
	void Walk(float d);

	void Pitch(float angle);
	void RotationY(float angle);

	void Update();
	VOID Moving(float delta);
};

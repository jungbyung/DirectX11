#pragma once

class Object
{
protected:
	XMFLOAT3 mRight;
	XMFLOAT3 mUp;
	XMFLOAT3 mForward;

	XMFLOAT3 mPosition;
	XMFLOAT3 mScale;

	XMFLOAT4X4 mLocal;
	XMFLOAT4X4 mWorld;

	Object* mParent;
	vector<Object*> mChilds;

	Material mMaterial;
public:
	Object();
	virtual ~Object();

	virtual void Update(const float deltaTime = 0.0f);

	void SetPosition(FXMVECTOR pos);
	void MovePosition(FXMVECTOR delta);
	void SetScale(FXMVECTOR scale);
	void SetRotation(FXMVECTOR rot);

	void LookDirection(FXMVECTOR dir, FXMVECTOR up = XMVectorSet(0, 1, 0, 1));
	void LookPosition(FXMVECTOR pos, FXMVECTOR up = XMVectorSet(0, 1, 0, 1));

	void Walk(float delta);
	void Strafe(float delta);

	void Pitch(float angle);
	void RotationY(float angle);

	VOID MouseMove(POINT pt);

	VOID Moving(float delta = 0.0f);

	const XMFLOAT3& GetPosition() { return mPosition; }
	const XMFLOAT3& GetForward() { return mForward; }
	const XMFLOAT3& GetScale() { return mScale; }
	const XMFLOAT3& GetUp() { return mUp; }
	const XMFLOAT3& GetRight() { return mRight; }

	const XMFLOAT4X4& GetLocalMatrix() { return mLocal; }
	const XMFLOAT4X4& GetWorldMatrix() { return mWorld; }

	Object* GetThis() { return this; }
};

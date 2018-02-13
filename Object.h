#pragma once

class Object
{
protected:
	XMFLOAT3 mRight;
	XMFLOAT3 mUp;
	XMFLOAT3 mForward;

	XMFLOAT3 mPosition;
	XMFLOAT3 mScale;

	XMMATRIX mLocal;
	XMMATRIX mWorld;

	Object* mParent;
public:
	Object();
	virtual ~Object();

	virtual void Update();

	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& pos);

	void MovePosition(float x, float y, float z);
	void MovePosition(const XMFLOAT3& delta);

	void SetScale(float x, float y, float z);
	void SetScale(const XMFLOAT3& scale);

	void SetRotation(float x, float y, float z);
	void SetRotation(const XMFLOAT3& rot);

	void LookDirection(const XMFLOAT3& dir, const XMFLOAT3& up = XMFLOAT3(0, 1, 0));
	void LookPosition(const XMFLOAT3& pos, const XMFLOAT3& up = XMFLOAT3(0, 1, 0));

	void Walk(float delta);
	void Strafe(float delta);

	void Pitch(float angle);
	void RotationY(float angle);

	VOID MouseMove(POINT pt);

	XMFLOAT3 GetPosition() { return mPosition; }
	XMFLOAT3 GetForward() { return mForward; }
	XMFLOAT3 GetScale() { return mScale; }
	XMFLOAT3 GetUp() { return mUp; }
	XMFLOAT3 GetRight() { return mRight; }

	XMMATRIX& GetLocalMatrix() { return mLocal; }
	XMMATRIX& GetWorldMatrix() { return mWorld; }

	Object* GetObject() { return this; }
};

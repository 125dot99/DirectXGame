#pragma once
#include "../math/MyMath.h"
#include "../math/Matrix4.h"
#include "../dx12/ConstBuffer.h"
#include "../collision/BaseCollider.h"

namespace gamelib
{
class GameObject
{
private:
	Matrix4 matWorld;

	std::weak_ptr<BaseCollider> w_pCollider;
	std::unique_ptr<ConstBuffer> u_pConstBuffer;

protected:
	bool isActive;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	GameObject* pParentObject;

	inline void UpdateWorldMatrix()
	{
		matWorld = MatrixScale(scale);
		matWorld *= MatrixRotateZ(ConvertToRadian(rotation.z));
		matWorld *= MatrixRotateX(ConvertToRadian(rotation.x));
		matWorld *=	MatrixRotateY(ConvertToRadian(rotation.y));
		matWorld *= MatrixTranslate(position);
		if (pParentObject)//親オブジェクトのワールド行列と合成
		{
			matWorld *= pParentObject->matWorld;
		}
		u_pConstBuffer->Map(&matWorld);
	}

	inline void UpdateCollider()
	{
		if (w_pCollider.lock())
		{
			w_pCollider.lock()->Update();
		}
	}

public:
	inline GameObject() : isActive(true), scale(Vector3::One()), pParentObject(nullptr) 
	{
		u_pConstBuffer = std::make_unique<ConstBuffer>();
		u_pConstBuffer->Init(0, sizeof(Matrix4));
	}
	
	inline GameObject(
		const Vector3& _position,
		const Vector3& _rotation = { 0,0,0 }, 
		const Vector3& _scale = { 1,1,1 }, 
		GameObject* pParent = nullptr) : GameObject()
	{
		position = _position;
		rotation = _rotation;
		scale = _scale;
		pParentObject = pParent;
	}

	virtual ~GameObject() = default;
	
	virtual inline void Initialize() {}
	
	virtual inline void Update()
	{
		if (!isActive)
		{
			return;
		}
		UpdateWorldMatrix(); 
		UpdateCollider();
	}

	virtual inline void OnCollision(BaseCollider* collA, BaseCollider* collB) {}
	
	inline void GraphicsCommand() const { u_pConstBuffer->GraphicsCommand(); }

	// setter
	inline void SetPosition(const Vector3& _position) { position = _position; }
	inline void SetRotation(const Vector3& _rotation) { rotation = _rotation; }
	inline void SetScale(const Vector3& _scale) { scale = _scale; }
	
	void SetActive(bool _isActive);
	void SetParent(GameObject* pGameObject);
	void SetCollider(std::weak_ptr<BaseCollider> pCollider);

	// getter
	inline const Vector3& GetPosition() const { return position; }
	inline const Vector3& GetRotation() const { return rotation; }
	inline const Vector3& GetScale() const { return scale; }
	inline const Matrix4& GetMatrixWorld() const { return matWorld; }
	inline const Vector3 GetWorldPosition() const { return Vector3(matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2]); }
};
} // namespace gamelib
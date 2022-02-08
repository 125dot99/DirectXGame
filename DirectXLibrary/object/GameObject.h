#pragma once
#include "../math/MyMath.h"
#include "../math/Matrix4.h"
#include "../dx12/ConstBuffer.h"
#include "../dx12/IMesh.h"
#include "../collision/BaseCollider.h"

namespace gamelib
{
class GameObject
{
private:
	Matrix4 matWorld;
	GameObject* parent;

	std::weak_ptr<IMesh> mesh;
	std::weak_ptr<BaseCollider> collider;

	std::unique_ptr<ConstBuffer> cbuffer;

protected:
	bool isActive;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	inline void UpdateWorldMatrix()
	{
		matWorld = MatrixScale(scale);
		matWorld *= MatrixRotateZ(ConvertToRadian(rotation.z));
		matWorld *= MatrixRotateX(ConvertToRadian(rotation.x));
		matWorld *=	MatrixRotateY(ConvertToRadian(rotation.y));
		matWorld *= MatrixTranslate(position);
		if (parent)//親オブジェクトのワールド行列と合成
		{
			matWorld *= parent->matWorld;
		}
	}

	inline void UpdateCollider()
	{
		if (collider.lock())
		{
			collider.lock()->Update();
		}
	}

public:
	inline GameObject() : isActive(true), scale(Vector3::One()), parent(nullptr) 
	{
		cbuffer = std::make_unique<ConstBuffer>();
		cbuffer->Init(0, sizeof(Matrix4));
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
		parent = pParent;
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
	
	inline void TransferBuffer() { cbuffer->Map(&matWorld); }

	inline void GraphicsCommand() const { cbuffer->GraphicsCommand(); }

	// setter
	inline void SetPosition(const Vector3& _position) { position = _position; }
	inline void SetRotation(const Vector3& _rotation) { rotation = _rotation; }
	inline void SetScale(const Vector3& _scale) { scale = _scale; }
	
	void SetActive(bool _isActive);
	void SetMeshFile(const char* meshFile);
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
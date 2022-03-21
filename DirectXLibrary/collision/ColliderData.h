#pragma once
#include <vector>

#include "BaseCollider.h"
#include "Collision.h"
#include "CollisionPrimitive.h"
#include "../object/GameObject.h"
#include "../dx12/IMesh.h"

namespace gamelib
{
class Box2DCollider : public BaseCollider, public primitive::Box2D
{
private:
	Vector2 offset;
	Vector2 size;
public:
	Box2DCollider(const Vector2& offset, const Vector2& size) : offset(offset), size(size) {}
	inline void Update() override
	{
		Vector2 pos = pGameObject->GetPosition();
		pos += offset;
		minPosition = pos - size;
		maxPosition = pos + size;
	}
	inline bool Dispatch(BaseCollider& collider) override
	{
		return collider.IsCollision(*this);
	}
	inline bool IsCollision(primitive::Box2D& box) override 
	{ 
		return Collision::CheckHitBox2D_Box2D(*this, box);
	}
	inline bool IsCollision(primitive::Sphere2D& sphere) override { return false; }
	inline bool IsCollision(primitive::Box& box) override { return false; }
	inline bool IsCollision(primitive::Sphere& sphere) override { return false; }
	inline PRIMITIVE_TYPE GetType() const override { return PRIMITIVE_TYPE::BOX2D; }
};

class Sphere2DCollider : public BaseCollider, public primitive::Sphere2D
{
private:
	Vector2 offset;
public:
	Sphere2DCollider(const Vector2& offset, const float radius) : offset(offset) 
	{
		this->radius = radius;
	}
	inline void Update() override
	{
		Vector2 pos = pGameObject->GetPosition();
		center = pos + offset;
	}
	inline bool Dispatch(BaseCollider& collider) override
	{
		return collider.IsCollision(*this);
	}
	inline bool IsCollision(primitive::Box2D& box) override { return false; }
	inline bool IsCollision(primitive::Sphere2D& sphere) override 
	{ 
		return Collision::CheckHitSphere2D_Sphere2D(*this, sphere);
	}
	inline bool IsCollision(primitive::Box& box) override { return false; }
	inline bool IsCollision(primitive::Sphere& sphere) override { return false; }

	inline PRIMITIVE_TYPE GetType() const override { return PRIMITIVE_TYPE::SPHERE2D; }
};

class BoxCollider : public BaseCollider, public primitive::Box
{
private:
	Vector3 offset;
	Vector3 size;
public:
	BoxCollider(const Vector3& offset, const Vector3& size) : offset(offset), size(size) {}
	inline void Update()
	{
		Vector3 pos = pGameObject->GetWorldPosition();
		pos += offset;
		minPosition = pos - size;
		maxPosition = pos + size;
	}
	inline bool Dispatch(BaseCollider& collider) override
	{
		return collider.IsCollision(*this);
	}
	inline bool IsCollision(primitive::Box2D& box) override { return false; }
	inline bool IsCollision(primitive::Sphere2D& sphere) override { return false; }
	inline bool IsCollision(primitive::Box& box) override
	{
		return Collision::CheckHitBox_Box(*this, box);
	}
	inline bool IsCollision(primitive::Sphere& sphere) override 
	{ 
		return Collision::CheckHitSphere_Box(sphere, *this);
	}
	inline PRIMITIVE_TYPE GetType() const override  { return PRIMITIVE_TYPE::BOX; }
};

class SphereCollider : public BaseCollider, public primitive::Sphere
{
private:
	Vector3 offset;
public:
	SphereCollider(const Vector3& offset, const float radius) : offset(offset)
	{
		this->radius = radius;
	}
	inline void Update() override
	{
		Vector3 pos = pGameObject->GetWorldPosition();
		center = pos + offset;
	}
	inline bool Dispatch(BaseCollider& collider) override
	{
		return collider.IsCollision(*this);
	}
	inline bool IsCollision(primitive::Box2D& box) override { return false; }
	inline bool IsCollision(primitive::Sphere2D& sphere) override { return false; }
	inline bool IsCollision(primitive::Box& box) override
	{
		return Collision::CheckHitSphere_Box(*this, box);
	}

	inline bool IsCollision(primitive::Sphere& sphere) override
	{
		return Collision::CheckHitSphere_Sphere(*this, sphere);
	}
	inline PRIMITIVE_TYPE GetType() const override { return PRIMITIVE_TYPE::SPHERE; }
};

class MeshCollider : public BaseCollider
{
private:
	std::vector<primitive::Triangle> triangles;
	Matrix4 invMatWorld;
public:
	MeshCollider(std::weak_ptr<IMesh> mesh);
	inline void Update()
	{
		invMatWorld = MatrixInverse(pGameObject->GetMatrixWorld());
	}
	inline bool Dispatch(BaseCollider& collider) override { return false; }
	inline bool IsCollision(primitive::Box2D& box) override { return false; }
	inline bool IsCollision(primitive::Sphere2D& sphere) override { return false; }
	inline bool IsCollision(primitive::Box& box) override { return false; }
	inline bool IsCollision(primitive::Sphere& sphere) override { return false; }
	inline PRIMITIVE_TYPE GetType() const override { return PRIMITIVE_TYPE::MESH; }
};
} // namespace gamelib
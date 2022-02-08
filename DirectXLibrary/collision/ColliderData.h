#pragma once
#include "CollisionPrimitive.h"
#include "BaseCollider.h"
#include "../object/GameObject.h"
#include "../dx12/IMesh.h"
#include <vector>

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
		Vector2 pos = gameObject->GetPosition();
		pos += offset;
		minPosition = pos - size;
		maxPosition = pos + size;
	}
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
		Vector2 pos = gameObject->GetPosition();
		center = pos + offset;
	}
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
		Vector3 pos = gameObject->GetWorldPosition();
		pos += offset;
		minPosition = pos - size;
		maxPosition = pos + size;
	}
	inline PRIMITIVE_TYPE GetType() const override { return PRIMITIVE_TYPE::BOX; }
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
		Vector3 pos = gameObject->GetWorldPosition();
		center = pos + offset;
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
		invMatWorld = MatrixInverse(gameObject->GetMatrixWorld());
	}
	inline PRIMITIVE_TYPE GetType() const override { return PRIMITIVE_TYPE::MESH; }
};
} // namespace gamelib
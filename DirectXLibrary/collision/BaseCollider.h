#pragma once
#include "CollisionPrimitive.h"

namespace gamelib
{
enum class PRIMITIVE_TYPE
{
	BOX2D,
	SPHERE2D,
	BOX,
	SPHERE,
	CAPSULE,
	MESH,
};

class BaseCollider abstract
{
private:
	friend class CollisionManager;
	bool isCheck;
	unsigned short attribute;

protected:
	class GameObject* pGameObject;

	/// <summary>
	/// コライダーの型変換
	/// </summary>
public:
	BaseCollider() : pGameObject(nullptr), attribute(0), isCheck(true) {}
	virtual ~BaseCollider() = default;

	/// <summary>
	/// コライダーの更新
	/// </summary>
	virtual void Update() = 0;

	virtual bool Dispatch(BaseCollider& collider) = 0;
	
	virtual bool IsCollision(primitive::Box2D& box) = 0;
	virtual bool IsCollision(primitive::Sphere2D& sphere) = 0;
	virtual bool IsCollision(primitive::Box& box) = 0;
	virtual bool IsCollision(primitive::Sphere& sphere) = 0;

	/// <summary>
	/// コライダーの判別
	/// </summary>
	/// <returns></returns>
	virtual PRIMITIVE_TYPE GetType() const = 0;
};
} // namespace gamelib

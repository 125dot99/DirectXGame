#pragma once
#include "BaseCollider.h"
#include "../math/Vector.h"

#include "CollisionPrimitive.h"

namespace gamelib
{
class CollisionHelper
{
private:
	CollisionHelper() = delete;
	~CollisionHelper() = delete;
public:
	//â~ÇÃîÕàÕÇ©ÇÁèoÇ»Ç¢ÇÊÇ§Ç…âüÇµñﬂÇ∑
	static Vector3 PushInPointRangeCircle(const Vector3& pos, float worldRadius);
	static Vector3 PushInPointRangeRect(const Vector3& pos, const primitive::Box& box);
	static Vector3 PushOutPointRangeRect(const Vector3& pos, const primitive::Box& box);
	static Vector2 PushBox2DHitBox2D(BaseCollider* collA, BaseCollider* collB);
	static Vector3 PushSphereHitSphere(BaseCollider* collA, BaseCollider* collB);
};
} // namespace gamelib

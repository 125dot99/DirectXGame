#include "CollisionHelper.h"

#include <algorithm>
#include "ColliderData.h"
#include "Collision.h"

using namespace gamelib::primitive;

gamelib::Vector3 gamelib::CollisionHelper::PushInPointRangeCircle(const Vector3& pos, float worldRadius)
{
	float dis = pos.SqrtLength();
	if (dis < worldRadius)
	{
		return Vector3::Zero();
	}
	return pos.Normalize() * (worldRadius - dis);
}

gamelib::Vector3 gamelib::CollisionHelper::PushInPointRangeRect(const Vector3& pos, const Box& box)
{
	if (Collision::CheckHitPoint_Box(pos, box))
	{
		return Vector3::Zero();
	}
	Vector3 result;
	if (pos.x > box.maxPosition.x)
	{
		result.x = box.maxPosition.x - pos.x;
	}
	else if (pos.x < box.minPosition.x)
	{
		result.x = box.minPosition.x - pos.x;
	}
	if (pos.z > box.maxPosition.z)
	{
		result.z = box.maxPosition.z - pos.z;
	} 
	else if (pos.z < box.minPosition.z)
	{
		result.z = box.minPosition.z - pos.z;
	}
	return result;
}

gamelib::Vector3 gamelib::CollisionHelper::PushOutPointRangeRect(const Vector3& pos, const primitive::Box& box)
{
	if (!Collision::CheckHitPoint_Box(pos, box))
	{
		return Vector3::Zero();
	}
	Vector3 result;
	float x1 = box.minPosition.x - pos.x;
	float x2 = box.maxPosition.x - pos.x;
	float z1 = box.minPosition.z - pos.z;
	float z2 = box.maxPosition.z - pos.z;
	float x3 = std::abs(x1);
	float x4 = std::abs(x2);
	float z3 = std::abs(z1);
	float z4 = std::abs(z2);
	float minx = Max(x3, x4);
	float minz = Max(z3, z4);
	if (x3 < x4 && x3 < minz)
	{
		result.x = x1;
	}
	else if (x4 < x3 && x4 < minz)
	{
		result.x = x2;
	}
	if (z3 < z4 && z3 < minx)
	{
		result.z = z1;
	}
	else if (z4 < z3 && z4 < minx)
	{
		result.z = z2;
	}
	return result;
}

gamelib::Vector2 gamelib::CollisionHelper::PushBox2DHitBox2D(BaseCollider* collA, BaseCollider* collB)
{
	Box2D* boxA = dynamic_cast<Box2D*>(collA);
	Box2D* boxB = dynamic_cast<Box2D*>(collB);
	Vector2 result;
	float x1 = boxB->minPosition.x - boxA->maxPosition.x;
	float x2 = boxB->maxPosition.x - boxA->minPosition.x;
	float y1 = boxB->minPosition.y - boxA->maxPosition.y;
	float y2 = boxB->maxPosition.y - boxA->minPosition.y;
	float x3 = std::abs(x1);
	float x4 = std::abs(x2);
	float y3 = std::abs(y1);
	float y4 = std::abs(y2);
	float minx = Max(x3, x4);
	float miny = Max(y3, y4);
	if (x3 < x4 && x3 < miny)
	{
		result.x = x1;
	} 
	else if (x4 < x3 && x4 < miny)
	{
		result.x = x2;
	}
	if (y3 < y4 && y3 < minx)
	{
		result.y = y1;
	}
	else if (y4 < y3 && y4 < minx)
	{
		result.y = y2;
	}
	return result;
}

gamelib::Vector3 gamelib::CollisionHelper::PushSphereHitSphere(BaseCollider* collA, BaseCollider* collB)
{
	Sphere* sphereA = dynamic_cast<Sphere*>(collA);
	Sphere* sphereB = dynamic_cast<Sphere*>(collB);
	Vector3 v = sphereA->center - sphereB->center;
	float len = sphereA->radius + sphereB->radius - v.SqrtLength();
    return v.Normalize() * len;
}


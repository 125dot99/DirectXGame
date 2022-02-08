#pragma once
#include "../math/Vector.h"

namespace gamelib
{
namespace primitive
{
struct Sphere2D
{
	Vector2 center;
	float radius;
};

struct Box2D
{
	Vector2 minPosition;
	Vector2 maxPosition;
};

struct Box
{
	Vector3 minPosition;
	Vector3 maxPosition;
};

struct Sphere
{
	Vector3 center;
	float radius;
};

struct Ray
{
	Vector3 start;
	Vector3 end;
};

struct Capsule
{
	Vector3 start;
	Vector3 end;
	float radius;
};

struct Plane
{
	Vector3 normal;
	float distance;
};

struct Triangle
{
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;
	Vector3 normal;
	void ComputeNormal()
	{
		Vector3 p0_p1 = p1 - p0;
		Vector3 p0_p2 = p2 - p0;
		normal = (p0_p1.Cross(p0_p2)).Normalize();
	}
};
} // namespace primitive
} // namespace gamelib
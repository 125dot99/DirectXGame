#pragma once
#include "CollisionPrimitive.h"
#include "../math/MyMath.h"

namespace gamelib
{
using namespace primitive;

class Collision
{
private:
	Collision() = delete;
	~Collision() = delete;

	static void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest);
	//線分と線分の距離の2乗を求める
	static float SqDistanceSegmentSegment(const Vector3& p1, const Vector3& q1, const Vector3& p2, const Vector3& q2);
public:
	//2Dの点と球の衝突判定
	inline static bool CheckHitPoint_Sphere2D(const Vector2& point, const Sphere2D& sphere)
	{
		Vector2 centor = sphere.center - point;
		return centor.Length() < sphere.radius;
	}
	//点と四角形の衝突判定
	inline static bool CheckHitPoint_Box(const Vector3& point, const Box& box)
	{
		return point.x > box.minPosition.x &&
			point.x < box.maxPosition.x &&
			point.y > box.minPosition.y &&
			point.y < box.maxPosition.y &&
			point.z > box.minPosition.z &&
			point.z < box.maxPosition.z;
	}
	//点と球の衝突判定
	inline static bool CheckHitPoint_Sphere(const Vector3& point, const Sphere& sphere)
	{
		Vector3 centor = sphere.center - point;
		return centor.Length() < sphere.radius;
	}
	//2Dの四角形同士の衝突判定
	inline static bool CheckHitBox2D_Box2D(const Box2D& box1, const Box2D& box2)
	{
		return box1.maxPosition.x > box2.minPosition.x &&
			box1.minPosition.x < box2.maxPosition.x &&
			box1.maxPosition.y > box2.minPosition.y &&
			box1.minPosition.y < box2.maxPosition.y;
	}
	//2Dの円同士の衝突判定
	inline static bool CheckHitSphere2D_Sphere2D(const Sphere2D& sphere1, const Sphere2D& sphere2)
	{
		Vector2 p = sphere1.center - sphere2.center;
		float r = sphere1.radius + sphere2.radius;
		return p.Length() < r * r;
	}
	//四角形同士の衝突判定
	inline static bool CheckHitBox_Box(const Box& box1, const Box& box2)
	{
		return box1.maxPosition.x > box2.minPosition.x &&
			box1.minPosition.x < box2.maxPosition.x &&
			box1.maxPosition.y > box2.minPosition.y &&
			box1.minPosition.y < box2.maxPosition.y && 
			box1.maxPosition.z > box2.minPosition.z &&
			box1.minPosition.z < box2.maxPosition.z;
	}
	//球と球の衝突判定
	inline static bool CheckHitSphere_Sphere(const Sphere& sphere1, const Sphere& sphere2)
	{
		Vector3 pos = sphere1.center - sphere2.center;
		float r = sphere1.radius + sphere2.radius;
		return pos.Length() < r * r;
	}
	//球と平面の衝突判定
	static bool CheckHitSphere_Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);
	//三角形と球の判定
	static bool CheckHitSphere_Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);
	//レイと平面の交差判定
	static bool CheckHitRay_Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);
	//レイと三角形の交差判定
	static bool CheckHitRay_Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, Vector3* inter = nullptr);
	//レイと球の交差判定
	static bool CheckHitRay_Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, Vector3* inter = nullptr);
	//線と円の衝突判定xy平面
	static bool CheckHitRay_CircleXY(const Ray& ray, const Sphere& sphere);
	//線と円の衝突判定xz平面
	static bool CheckHitRay_CircleXZ(const Ray& ray, const Sphere& sphere);
	//レイと球の衝突判定
	static bool CheckHitRay_Sphere(const Ray& ray, const Sphere& sphere);
	//球と直方体の衝突判定
	static bool CheckHitSphere_Box(const Sphere& sphere, const Box& box);
	//球とカプセルの衝突判定
	static bool CheckHitSphere_Capsule(const Sphere& sphere, const Capsule& capsule);
	//カプセルとカプセルの衝突判定
	static bool CheckHitCapsule_Capsule(const Capsule& capsule1, const Capsule& capsule2);
};
} // namespace gamelib

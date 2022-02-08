#include "Collision.h"

void gamelib::Collision::ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	Vector3 p0_p1 = triangle.p1 - triangle.p0;
	Vector3 p0_p2 = triangle.p2 - triangle.p0;
	Vector3 p0_pt = point - triangle.p0;
	float d1 = p0_p1.Dot(p0_pt);
	float d2 = p0_p2.Dot(p0_pt);
	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0;
		return;
	}
	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	Vector3 p1_pt = point - triangle.p1;
	float d3 = p0_p1.Dot(p1_pt);
	float d4 = p0_p2.Dot(p1_pt);
	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1が最近傍
		*closest = triangle.p1;
		return;
	}
	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}
	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	Vector3 p2_pt = point - triangle.p2;
	float d5 = p0_p1.Dot(p2_pt);
	float d6 = p0_p2.Dot(p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.p2;
		return;
	}
	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}
	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

float gamelib::Collision::SqDistanceSegmentSegment(const Vector3& p1, const Vector3& q1, const Vector3& p2, const Vector3& q2)
{
	Vector3 d1 = q1 - p1;
	Vector3 d2 = q2 - p2;
	//半径
	Vector3 r = p1 - p2;
	float a = d1.Dot(d1);
	float b = d1.Dot(d2);
	float e = d2.Dot(d2);

	float c = d1.Dot(r);
	float f = d2.Dot(r);

	float s = 0.0f;
	float t = 0.0f;
	float denominator = a * e - (b * b);	//分母
	//sを求める
	if (denominator != 0.0f)//2つの線分が平行でないなら
	{
		s = (b * f - c * e) / denominator;
		s = Clamp(s, 0.0f, 1.0f);
	}
	else//2つの線分が平行であるとき
	{
		s = 0.0f;
	}
	//tを求める
	t = (f + b * s) / e;
	//t が [0.0, 1.0]の範囲外であれば、sを再計算する
	if (t < 0.0f)
	{
		t = 0.0f;
		s = Clamp(-c / a, 0.0f, 1.0f);
	}
	else if (t > 1.0f)
	{
		s = Clamp((b - c) / a, 0.0f, 1.0f);
		t = 1.0f;
	}
	//s と t の値が決定したので、各線分内の座標c1 と c2を求める
	Vector3 c1 = p1 + s * d1;
	Vector3 c2 = p2 + t * d2;
	//2点間(c1　と　c2) の距離の2乗を求めて、結果を返す
	return (c1 - c2).Dot(c1 - c2);
}

bool gamelib::Collision::CheckHitSphere_Plane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
	//球の中心座標への距離
	float distV = sphere.center.Dot(plane.normal);
	float dist = distV - plane.distance;
	if (inter)
	{
		*inter = -dist * plane.normal + sphere.center;
	}
	return fabsf(dist) <= sphere.radius;
}

bool gamelib::Collision::CheckHitSphere_Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter)
{
	//三角形上にある点を見つける
	Vector3 p;
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	Vector3 v = p - sphere.center;
	float d = v.Dot(v);
	if (d > sphere.radius * sphere.radius)
	{
		return false;
	}
	if (inter)
	{
		*inter = p;
	}
	return true;
}

bool gamelib::Collision::CheckHitRay_Plane(const Ray& ray, const Plane& plane, float* distance, Vector3* inter)
{
	//誤差吸収用
	const float epsilon = 1.0e-5f;
	Vector3 direction = (ray.end - ray.start).Normalize();
	//面法線とレイの方向ベクトルの内積
	float d1 = plane.normal.Dot(direction);
	if (d1 > -epsilon)
	{
		//裏面には当たらない
		return false;
	}
	//面法線とレイの始点座標（位置ベクトルの内積）
	float d2 = plane.normal.Dot(ray.start);
	//始点と原点の距離（平面の法線方向）
	float dist = d2 - plane.distance;
	//始点と平面の距離（レイ方向）
	float t = dist / -d1;
	//交点が始点より後ろにあるので、当たらない
	if (t < 0)
	{
		return false;
	}
	if (distance)
	{
		*distance = t;
	}
	if (inter)
	{
		*inter = ray.start + t * direction;
	}
	return true;
}

bool gamelib::Collision::CheckHitRay_Triangle(const Ray& ray, const Triangle& triangle, float* distance, Vector3* inter)
{
	Plane plane;
	Vector3 interPlane;
	plane.normal = triangle.normal;
	plane.distance = triangle.normal.Dot(triangle.p0);
	//レイと平面が当たっていなければ、当たっていない
	if (!CheckHitRay_Plane(ray, plane, distance, &interPlane))
	{
		return false;
	}
	//レイと平面の交点が三角形の内側にあるか判定
	//誤差吸収用
	const float epsilon = 1.0e-5f;
	//p0_p1
	Vector3 pt_p0 = triangle.p0 - interPlane;
	Vector3 pt0_p1 = triangle.p1 - triangle.p0;
	Vector3 m = pt_p0.Cross(pt0_p1);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (m.Dot(triangle.normal) < -epsilon)
	{
		return false;
	}
	//p1_p2
	Vector3 pt_p1 = triangle.p1 - interPlane;
	Vector3 pt1_p2 = triangle.p2 - triangle.p1;
	m = pt_p1.Cross(pt1_p2);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (m.Dot(triangle.normal) < -epsilon)
	{
		return false;
	}
	Vector3 pt_p2 = triangle.p2 - interPlane;
	Vector3 pt2_p0 = triangle.p0 - triangle.p2;
	m = pt_p2.Cross(pt2_p0);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (m.Dot(triangle.normal) < -epsilon)
	{
		return false;
	}
	if (inter)
	{
		*inter = interPlane;
	}
	return true;
}

bool gamelib::Collision::CheckHitRay_Sphere(const Ray& ray, const Sphere& sphere, float* distance, Vector3* inter)
{
	//線分のベクトル
	Vector3 direction = (ray.end - ray.start).Normalize();
	Vector3 m = ray.start - sphere.center;
	float b = m.Dot(direction);
	float c = m.Dot(m) - sphere.radius * sphere.radius;
	//rayの始点がsphereの外側にあり(c > 0)、rayがsphereから離していく方向を指している場合(b > 0)、当たらない
	if (c > 0.0f && b > 0.0f)
	{
		return false;
	}
	float discr = b * b - c;
	//負の判別式はレイが球を外れていることに一致
	if (discr < 0.0f)
	{
		return false;
	}
	float t = -b - sqrtf(discr);
	if (t < 0)
	{
		t = 0.0f;
	}
	if (distance)
	{
		*distance = t;
	}
	if (inter)
	{
		*inter = ray.start + t * direction;
	}
	return true;
}

bool gamelib::Collision::CheckHitRay_CircleXY(const Ray& ray, const Sphere& sphere)
{
	//線分のベクトル
	Vector3 va = ray.end - ray.start;
	//始点から球の中心
	Vector3 vb = sphere.center - ray.start;
	//終点から球の中心
	Vector3 vc = sphere.center - ray.end;
	float dist = va.x * vb.y - vb.x * va.y;
	//距離が半径より長ければ当たっていない
	if (fabsf(dist / va.SqrtLength()) > sphere.radius)
	{
		return false;
	}
	//円の範囲内にあるか
	if (vb.SqrtLength() <= sphere.radius ||
		vc.SqrtLength() <= sphere.radius)
	{
		return true;
	}
	float dotb = vb.Dot(va);
	float dotc = vc.Dot(va);
	//符号が等しい
	return signbit(dotb) != signbit(dotc);
}

bool gamelib::Collision::CheckHitRay_CircleXZ(const Ray& ray, const Sphere& sphere)
{
	//線分
	Vector3 va = ray.end - ray.start;
	//始点から球の中心
	Vector3 vb = sphere.center - ray.start;
	//終点から球の中心
	Vector3 vc = sphere.center - ray.end;
	float dist = va.x * vb.z - vb.x * va.z;
	//距離が半径より長ければ当たっていない
	if (fabsf(dist / va.SqrtLength()) > sphere.radius)
	{
		return false;
	}
	//円の範囲内にあるか
	if (vb.SqrtLength() <= sphere.radius ||
		vc.SqrtLength() <= sphere.radius)
	{
		return true;
	}
	float dotb = vb.Dot(va);
	float dotc = vc.Dot(va);
	if (signbit(dotb) == signbit(dotc))
	{
		return false;
	}
	return true;
}

bool gamelib::Collision::CheckHitRay_Sphere(const Ray& ray, const Sphere& sphere)
{
	return false;
}

bool gamelib::Collision::CheckHitSphere_Box(const Sphere& sphere, const Box& box)
{
	float sqDistance = 0;
	float d = sphere.center.x;
	if (d < box.minPosition.x)
	{
		sqDistance += (d - box.minPosition.x) * (d - box.minPosition.x);
	}
	if (d > box.maxPosition.x)
	{
		sqDistance += (d - box.maxPosition.x) * (d - box.maxPosition.x);
	}
	d = sphere.center.y;
	if (d < box.minPosition.y)
	{
		sqDistance += (d - box.minPosition.y) * (d - box.minPosition.y);
	}
	if (d > box.maxPosition.y)
	{
		sqDistance += (d - box.maxPosition.y) * (d - box.maxPosition.y);
	}
	d = sphere.center.z;
	if (d < box.minPosition.z)
	{
		sqDistance += (d - box.minPosition.z) * (d - box.minPosition.z);
	}
	if (d > box.maxPosition.z)
	{
		sqDistance += (d - box.maxPosition.z) * (d - box.maxPosition.z);
	}
	return sqDistance < sphere.radius * sphere.radius;
}

bool gamelib::Collision::CheckHitSphere_Capsule(const Sphere& sphere, const Capsule& capsule)
{
	Vector3 vStartToEnd = capsule.end - capsule.start;
	Vector3 n = vStartToEnd.Normalize();
	float t = (sphere.center - capsule.start).Dot(n);
	//スタートから中心のベクトル
	Vector3 vPsPn = n * t;
	Vector3 posPn = vPsPn - capsule.end;
	float lengthRate = t / vStartToEnd.SqrtLength();
	float distance = 0;
	if (lengthRate < 0.0f)
	{
		//スタート位置の外側
		//カプセルのスタート位置と球の位置の距離
		distance = (capsule.start - sphere.center).SqrtLength() - capsule.radius;
	}
	else if (lengthRate <= 1.0f)
	{
		//カプセルの中
		distance = (posPn - sphere.center).SqrtLength() - capsule.radius;
	}
	else
	{
		//エンド位置の外側
		//カプセルのエンド位置と球の位置の距離
		distance = (capsule.end - sphere.center).SqrtLength() - capsule.radius;
	}
	return distance < sphere.radius;
}

bool gamelib::Collision::CheckHitCapsule_Capsule(const Capsule& capsule1, const Capsule& capsule2)
{
	float sqDistance = SqDistanceSegmentSegment(
		capsule1.start, 
		capsule1.end, 
		capsule2.start, 
		capsule2.end);
	return sqDistance < capsule1.radius + capsule2.radius;
}

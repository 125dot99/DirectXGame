#include "Collision.h"

void gamelib::Collision::ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p0_p1 = triangle.p1 - triangle.p0;
	Vector3 p0_p2 = triangle.p2 - triangle.p0;
	Vector3 p0_pt = point - triangle.p0;
	float d1 = p0_p1.Dot(p0_pt);
	float d2 = p0_p2.Dot(p0_pt);
	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p0;
		return;
	}
	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p1_pt = point - triangle.p1;
	float d3 = p0_p1.Dot(p1_pt);
	float d4 = p0_p2.Dot(p1_pt);
	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1���ŋߖT
		*closest = triangle.p1;
		return;
	}
	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}
	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p2_pt = point - triangle.p2;
	float d5 = p0_p1.Dot(p2_pt);
	float d6 = p0_p2.Dot(p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.p2;
		return;
	}
	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}
	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
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
	//���a
	Vector3 r = p1 - p2;
	float a = d1.Dot(d1);
	float b = d1.Dot(d2);
	float e = d2.Dot(d2);

	float c = d1.Dot(r);
	float f = d2.Dot(r);

	float s = 0.0f;
	float t = 0.0f;
	float denominator = a * e - (b * b);	//����
	//s�����߂�
	if (denominator != 0.0f)//2�̐��������s�łȂ��Ȃ�
	{
		s = (b * f - c * e) / denominator;
		s = Clamp(s, 0.0f, 1.0f);
	}
	else//2�̐��������s�ł���Ƃ�
	{
		s = 0.0f;
	}
	//t�����߂�
	t = (f + b * s) / e;
	//t �� [0.0, 1.0]�͈̔͊O�ł���΁As���Čv�Z����
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
	//s �� t �̒l�����肵���̂ŁA�e�������̍��Wc1 �� c2�����߂�
	Vector3 c1 = p1 + s * d1;
	Vector3 c2 = p2 + t * d2;
	//2�_��(c1�@�Ɓ@c2) �̋�����2������߂āA���ʂ�Ԃ�
	return (c1 - c2).Dot(c1 - c2);
}

bool gamelib::Collision::CheckHitSphere_Plane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
	//���̒��S���W�ւ̋���
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
	//�O�p�`��ɂ���_��������
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
	//�덷�z���p
	const float epsilon = 1.0e-5f;
	Vector3 direction = (ray.end - ray.start).Normalize();
	//�ʖ@���ƃ��C�̕����x�N�g���̓���
	float d1 = plane.normal.Dot(direction);
	if (d1 > -epsilon)
	{
		//���ʂɂ͓�����Ȃ�
		return false;
	}
	//�ʖ@���ƃ��C�̎n�_���W�i�ʒu�x�N�g���̓��ρj
	float d2 = plane.normal.Dot(ray.start);
	//�n�_�ƌ��_�̋����i���ʂ̖@�������j
	float dist = d2 - plane.distance;
	//�n�_�ƕ��ʂ̋����i���C�����j
	float t = dist / -d1;
	//��_���n�_�����ɂ���̂ŁA������Ȃ�
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
	//���C�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�
	if (!CheckHitRay_Plane(ray, plane, distance, &interPlane))
	{
		return false;
	}
	//���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	//�덷�z���p
	const float epsilon = 1.0e-5f;
	//p0_p1
	Vector3 pt_p0 = triangle.p0 - interPlane;
	Vector3 pt0_p1 = triangle.p1 - triangle.p0;
	Vector3 m = pt_p0.Cross(pt0_p1);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (m.Dot(triangle.normal) < -epsilon)
	{
		return false;
	}
	//p1_p2
	Vector3 pt_p1 = triangle.p1 - interPlane;
	Vector3 pt1_p2 = triangle.p2 - triangle.p1;
	m = pt_p1.Cross(pt1_p2);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (m.Dot(triangle.normal) < -epsilon)
	{
		return false;
	}
	Vector3 pt_p2 = triangle.p2 - interPlane;
	Vector3 pt2_p0 = triangle.p0 - triangle.p2;
	m = pt_p2.Cross(pt2_p0);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
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
	//�����̃x�N�g��
	Vector3 direction = (ray.end - ray.start).Normalize();
	Vector3 m = ray.start - sphere.center;
	float b = m.Dot(direction);
	float c = m.Dot(m) - sphere.radius * sphere.radius;
	//ray�̎n�_��sphere�̊O���ɂ���(c > 0)�Aray��sphere���痣���Ă����������w���Ă���ꍇ(b > 0)�A������Ȃ�
	if (c > 0.0f && b > 0.0f)
	{
		return false;
	}
	float discr = b * b - c;
	//���̔��ʎ��̓��C�������O��Ă��邱�ƂɈ�v
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
	//�����̃x�N�g��
	Vector3 va = ray.end - ray.start;
	//�n�_���狅�̒��S
	Vector3 vb = sphere.center - ray.start;
	//�I�_���狅�̒��S
	Vector3 vc = sphere.center - ray.end;
	float dist = va.x * vb.y - vb.x * va.y;
	//���������a��蒷����Γ������Ă��Ȃ�
	if (fabsf(dist / va.SqrtLength()) > sphere.radius)
	{
		return false;
	}
	//�~�͈͓̔��ɂ��邩
	if (vb.SqrtLength() <= sphere.radius ||
		vc.SqrtLength() <= sphere.radius)
	{
		return true;
	}
	float dotb = vb.Dot(va);
	float dotc = vc.Dot(va);
	//������������
	return signbit(dotb) != signbit(dotc);
}

bool gamelib::Collision::CheckHitRay_CircleXZ(const Ray& ray, const Sphere& sphere)
{
	//����
	Vector3 va = ray.end - ray.start;
	//�n�_���狅�̒��S
	Vector3 vb = sphere.center - ray.start;
	//�I�_���狅�̒��S
	Vector3 vc = sphere.center - ray.end;
	float dist = va.x * vb.z - vb.x * va.z;
	//���������a��蒷����Γ������Ă��Ȃ�
	if (fabsf(dist / va.SqrtLength()) > sphere.radius)
	{
		return false;
	}
	//�~�͈͓̔��ɂ��邩
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
	//�X�^�[�g���璆�S�̃x�N�g��
	Vector3 vPsPn = n * t;
	Vector3 posPn = vPsPn - capsule.end;
	float lengthRate = t / vStartToEnd.SqrtLength();
	float distance = 0;
	if (lengthRate < 0.0f)
	{
		//�X�^�[�g�ʒu�̊O��
		//�J�v�Z���̃X�^�[�g�ʒu�Ƌ��̈ʒu�̋���
		distance = (capsule.start - sphere.center).SqrtLength() - capsule.radius;
	}
	else if (lengthRate <= 1.0f)
	{
		//�J�v�Z���̒�
		distance = (posPn - sphere.center).SqrtLength() - capsule.radius;
	}
	else
	{
		//�G���h�ʒu�̊O��
		//�J�v�Z���̃G���h�ʒu�Ƌ��̈ʒu�̋���
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

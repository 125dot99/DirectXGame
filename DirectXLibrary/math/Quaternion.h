#pragma once
#include "Vector.h"
#include "Matrix4.h"
namespace gamelib
{
//クォータニオン
struct Quaternion
{
	float x;
	float y;
	float z;
	float w;

	//コンストラクタ
	Quaternion() : x(), y(), z(), w() {}
	Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	Quaternion(const Matrix4& m);
	Quaternion(const Vector3& v, const Quaternion& q);
	//内積を求める
	inline float Dot(const Quaternion& q) const { return w * q.w + x * q.x + y * q.y + z * q.z; }
	//ノルムを求める
	inline float SqrtLength() const { return sqrtf(Dot(*this)); }
	//共役(きょうやく)四元数を求める
	inline Quaternion Conjugate() const { return Quaternion(-x, -y, -z, w); }
	//Vector3で返す
	inline Vector3 Euler() const { return Vector3(x, y, z); }
	//正規化
	inline Quaternion Normalize() const;
	//代入演算子オーバーロード
	inline Quaternion& operator+=(const Quaternion& q) 
	{
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;
		return *this;
	}
	inline Quaternion& operator-=(const Quaternion& q)
	{
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;
		return *this;
	}
	inline Quaternion& operator*=(const float s)
	{
		w *= s;
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	inline Quaternion& operator/=(const float s)
	{
		return *this *= (1.0f / s);
	}
	inline Quaternion& operator*=(const Quaternion& q)
	{
		Quaternion result(
			 x * q.w + y * q.z - z * q.y + w * q.x,
			-x * q.z + y * q.w + z * q.x + w * q.y,
			 x * q.y - y * q.x + z * q.w + w * q.z,
			-x * q.x - y * q.y - z * q.z + w * q.w);
		return *this = result;
	}
};
//単項演算子オーバーロード
inline Quaternion operator+(const Quaternion& q) { return q; }
inline Quaternion operator-(const Quaternion& q) { return Quaternion(-q.x, -q.y, -q.z, -q.w); }
//2項演算子オーバーロード
inline Quaternion operator+(const Quaternion& q1, const Quaternion& q2) { return Quaternion(q1) += q2; }
inline Quaternion operator-(const Quaternion& q1, const Quaternion& q2) { return Quaternion(q1) -= q2; }
inline Quaternion operator*(const Quaternion& q1, const Quaternion& q2) { return Quaternion(q1) *= q2; }
inline Quaternion operator*(const Quaternion& q, const float s) { return Quaternion(q) *= s; }
inline Quaternion operator*(const float s, const Quaternion& q) { return Quaternion(q) *= s; }
inline Quaternion operator/(const Quaternion& q, const float s) { return Quaternion(q) /= s; }
//回転行列からクォータニオンを求める
inline Quaternion::Quaternion(const Matrix4& m)
{
	float tr = m.m[0][0] + m.m[1][1] + m.m[2][2] + m.m[3][3];
	if (tr >= 1.0f)
	{
		float fourD = 2.0f * sqrtf(tr);
		x = (m.m[1][2] - m.m[2][1]) / fourD;
		y = (m.m[2][0] - m.m[0][2]) / fourD;
		z = (m.m[0][1] - m.m[1][0]) / fourD;
		w = fourD / 4.0f;
	}
	int i = m.m[0][0] <= m.m[1][1] ? 1 : 0;
	if (m.m[2][2] > m.m[i][i])
	{
		i = 2;
	}
	int j = (i + 1) % 3;
	int k = (j + 1) % 3;
	tr = m.m[i][i] - m.m[j][j] - m.m[k][k] + 1.0f;
	float fourD = 2.0f * sqrtf(tr);
	float qa[4];
	qa[i] = fourD / 4.0f;
	qa[j] = (m.m[j][i] + m.m[i][j]) / fourD;
	qa[k] = (m.m[k][i] + m.m[i][k]) / fourD;
	qa[3] = (m.m[j][k] + m.m[k][j]) / fourD;
	*this = { qa[0], qa[1], qa[2], qa[3] };
}
//ベクトルをクォータニオンで回転しクォータニオンで返す
inline Quaternion::Quaternion(const Vector3& v, const Quaternion& q)
{
	Quaternion qv = Quaternion(v.x, v.y, v.z, 0.0f);
	*this = q * qv * q.Conjugate();
}
//正規化
inline Quaternion Quaternion::Normalize() const
{
	float len = SqrtLength();
	return len != 0.0f ? *this : *this / len;
}
//軸周りを回転
inline Quaternion AngleAxis(const Vector3& v, float angle) 
{
	float _sin = sinf(angle / 2.0f);
	return Quaternion(_sin * v.x, _sin * v.y, _sin * v.z, cosf(angle / 2.0f));
}
//回転の算出
inline Vector3 GetAxis(const Quaternion& q)
{ 
	return q.Euler() / q.SqrtLength(); 
}
//クォータニオンを球面線形補間する
inline Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	float cos = q1.Dot(q2);
	Quaternion t2 = q2;
	if (cos < 0.0f)
	{
		cos = -cos;
		t2 = -q2;
	}
	float k0 = 1.0f - t;
	float k1 = t;
	if (1.0f - cos > 0.001f)
	{
		float theta = acosf(cos);
		k0 = sinf(theta * k0) / sinf(theta);
		k1 = sinf(theta * k1) / sinf(theta);
	}
	return q1 * k0 + t2 * k1;
}
//クォータニオンを線形補間する
inline Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	float cos = q1.Dot(q2);
	Quaternion t2 = q2;
	if (cos < 0.0f)
	{
		cos = -cos;
		t2 = -q2;
	}
	float k0 = 1.0f - t;
	float k1 = t;
	return q1 * k0 + t2 * k1;
}
//クォータニオンから回転行列に変換する
inline Matrix4 MatrixRotate(const Quaternion& q)
{
	float xx = q.x * q.x * 2.0f;
	float yy = q.y * q.y * 2.0f;
	float zz = q.z * q.z * 2.0f;
	float xy = q.x * q.y * 2.0f;
	float xz = q.x * q.z * 2.0f;
	float yz = q.y * q.z * 2.0f;
	float wx = q.w * q.x * 2.0f;
	float wy = q.w * q.y * 2.0f;
	float wz = q.w * q.z * 2.0f;
	Matrix4 result;
	result.m[0][0] = 1.0f - yy - zz;
	result.m[0][1] = xy + wz;
	result.m[0][2] = xz - wy;
	result.m[1][0] = xy - wz;
	result.m[1][1] = 1.0f - xx - zz;
	result.m[1][2] = yz + wx;
	result.m[2][0] = xz + wy;
	result.m[2][1] = yz - wx;
	result.m[2][2] = 1.0f - xx - yy;
	result.m[3][3] = 1.0f;
	return result;
}
} // namespace gamelib

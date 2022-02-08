#pragma once
#include <cmath>
#include "MyMath.h"

namespace gamelib
{
//前方宣言
struct Vector3;
struct Vector4;

//2次元ベクトル
struct Vector2
{
	float x;
	float y;

	Vector2() : x(), y() {}
	Vector2(float x, float y) : x(x), y(y) {}
	Vector2(float v) : x(v), y(v) {}
	Vector2(const Vector3& v3);
	//内積を求める
	inline float Dot(const Vector2& v) const { return x * v.x + y * v.y; }
	//外積を求める
	inline float Cross(const Vector2& v) const { return x * v.y - y * v.x; }
	//長さを求める
	inline float Length() const { return Dot(*this); }
	//平方根の長さを求める
	inline float SqrtLength() const { return sqrtf(Length()); }
	//ベクトルが向いている角度を求める
	inline float ToAngle() const
	{
		return Length() != 0 ? ConvertToDegree(atan2f(y, x)) : 0.0f;
	}
	//角度からベクトルを求める
	static inline Vector2 FromAngle(const float degree)
	{
		float radian = ConvertToRadian(degree);
		return Vector2(cosf(radian), sinf(radian));
	}
	//正規化する
	inline Vector2 Normalize() const;

	inline Vector2& operator+=(const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	inline Vector2& operator-=(const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	inline Vector2& operator*=(const Vector2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}
	inline Vector2& operator/=(const Vector2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}
	inline Vector2& operator=(const float v)
	{
		x = y = v;
		return *this;
	}
	inline Vector2& operator*=(const float s)
	{
		x *= s;
		y *= s;
		return *this;
	}
	inline Vector2& operator/=(const float s)
	{
		return *this *= (1.0f / s);
	}
	//0ベクトル (0, 0)
	inline static const Vector2 Zero() 
	{
		static const Vector2 ZERO(0.0f, 0.0f);
		return ZERO;
	}
	//1ベクトル (1, 1)
	inline static const Vector2 One()
	{
		static const Vector2 ONE(1.0f, 1.0f);
		return ONE;
	}
	//右方向 (1, 0)
	inline static const Vector2 Rigth()
	{ 
		static const Vector2 RIGHT(1.0f, 0.0f);
		return RIGHT;
	}
	//上方向 (0, 1)
	inline static const Vector2 Up() 
	{ 
		static const Vector2 UP(0.0f, 1.0f);
		return UP;
	}
};
//単項演算子オーバーロード
inline Vector2 operator+(const Vector2& v) { return v; }
inline Vector2 operator-(const Vector2& v) { return Vector2(-v.x, -v.y); }
//二項演算子オーバーロード
inline Vector2 operator+(const Vector2& v1, const Vector2& v2) { return Vector2(v1) += v2; }
inline Vector2 operator-(const Vector2& v1, const Vector2& v2) { return Vector2(v1) -= v2; }
inline Vector2 operator*(const Vector2& v1, const Vector2& v2) { return Vector2(v1) *= v2; }
inline Vector2 operator/(const Vector2& v1, const Vector2& v2) { return Vector2(v1) /= v2; }
inline Vector2 operator*(const Vector2& v, const float s) { return Vector2(v) *= s; }
inline Vector2 operator*(const float s, const Vector2& v) { return Vector2(v) *= s; }
inline Vector2 operator/(const Vector2& v, const float s) { return Vector2(v) /= s; }
//正規化
inline Vector2 Vector2::Normalize() const
{
	const float len = SqrtLength();
	return len == 0 ? Zero() : *this / len;
}
//線形補間
inline Vector2 Lerp(const Vector2& start, const Vector2& end, const float t)
{
	return start + t * (end - start); 
}

//3次元ベクトル
struct Vector3
{
	float x;
	float y;
	float z;

	Vector3() : x(), y(), z() {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3(float v) : x(v), y(v), z(v) {}
	Vector3(const Vector2& v2);
	Vector3(const Vector4& v4);
	//内積を求める
	inline float Dot(const Vector3& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	//外積を求める
	inline Vector3 Cross(const Vector3& v) const
	{
		return Vector3(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x);
	}
	//長さを求める
	inline float Length() const { return Dot(*this); }
	//平方根の長さを求める
	inline float SqrtLength() const { return std::sqrtf(Length()); }
	//正規化する
	inline Vector3 Normalize() const;
	inline Vector3 Projection(const Vector3& v) const;
	inline Vector3 Rejection(const Vector3& v) const;
	inline Vector3& operator+=(const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	inline Vector3& operator-=(const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	inline Vector3& operator*=(const Vector3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	inline Vector3& operator/=(const Vector3& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}
	inline Vector3& operator=(const float v)
	{
		x = y = z = v;
		return *this;
	}
	inline Vector3& operator*=(const float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	inline Vector3& operator/=(const float s)
	{
		return *this *= (1.0f / s);
	}
	//0ベクトル (0, 0, 0)
	inline static const Vector3 Zero()
	{ 
		static const Vector3 ZERO(0.0f, 0.0f, 0.0f);
		return ZERO;
	}
	//1ベクトル (1, 1, 1)
	inline static const Vector3 One()
	{ 
		static const Vector3 ONE(1.0f, 1.0f, 1.0f);
		return ONE;
	}
	//右方向 (1, 0, 0)
	inline static const Vector3 Right()
	{
		static const Vector3 RIGHT(1.0f, 0.0f, 0.0f);
		return RIGHT;
	}
	//上方向 (0, 1, 0)
	inline static const Vector3 Up()
	{ 
		static const Vector3 UP(0.0f, 1.0f, 0.0f);
		return UP;
	}
	//前方向 (0, 0, 1)
	inline static const Vector3 Forward() 
	{ 
		static const Vector3 FORWARD(0.0f, 0.0f, 1.0f);
		return FORWARD;
	}
};
//単項演算子オーバーロード
inline Vector3 operator+(const Vector3& v) { return v; }
inline Vector3 operator-(const Vector3& v) { return Vector3(-v.x, -v.y, -v.z); }
//2項演算子オーバーロード
inline Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Vector3(v1) += v2; }
inline Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Vector3(v1) -= v2; }
inline Vector3 operator*(const Vector3& v1, const Vector3& v2) { return Vector3(v1) *= v2; }
inline Vector3 operator/(const Vector3& v1, const Vector3& v2) { return Vector3(v1) /= v2; }
inline Vector3 operator*(const Vector3& v, const float s) { return Vector3(v) *= s; }
inline Vector3 operator*(const float s, const Vector3& v) { return Vector3(v) *= s; }
inline Vector3 operator/(const Vector3& v, const float s) { return Vector3(v) /= s; }
//正規化
inline Vector3 Vector3::Normalize() const
{
	float len = SqrtLength();
	return len == 0.0f ? Zero() : *this / len;
}
inline Vector3 Vector3::Projection(const Vector3& v) const
{
	return (Dot(v) / v.Length()) * v;
}
inline Vector3 Vector3::Rejection(const Vector3& v) const
{
	return *this - Projection(v);
} 
//線形補間
inline Vector3 Lerp(const Vector3& start, const Vector3& end, const float t) 
{ 
	return start + t * (end - start); 
}

//4次元ベクトル
struct Vector4
{
	float x;
	float y;
	float z;
	float w;

	Vector4() : x(), y(), z(), w() {}
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	Vector4(float v) : x(v), y(v), z(v), w(v) {}
	Vector4(const Vector3& v3);

	//内積を求める
	inline float Dot(const Vector4& v) const
	{
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}

	inline Vector4& operator+=(const Vector4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
	inline Vector4& operator-=(const Vector4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}
	inline Vector4& operator*=(const Vector4& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}
	inline Vector4& operator/=(const Vector4& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}
	inline Vector4& operator=(const float v)
	{
		x = y = z = w = v;
		return *this;
	}
	inline Vector4& operator*=(const float s)
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}
	inline Vector4& operator/=(const float s)
	{
		return *this *= (1.0f / s);
	}
};
//単項演算子オーバーロード
inline Vector4 operator+(const Vector4& v) { return v; }
inline Vector4 operator-(const Vector4& v) { return Vector4(-v.x, -v.y, -v.z, -v.w); }
//2項演算子オーバーロード
inline Vector4 operator+(const Vector4& v1, const Vector4& v2) { return Vector4(v1) += v2; }
inline Vector4 operator-(const Vector4& v1, const Vector4& v2) { return Vector4(v1) -= v2; }
inline Vector4 operator*(const Vector4& v1, const Vector4& v2) { return Vector4(v1) *= v2; }
inline Vector4 operator/(const Vector4& v1, const Vector4& v2) { return Vector4(v1) /= v2; }
inline Vector4 operator*(const Vector4& v, const float s) { return Vector4(v) *= s; }
inline Vector4 operator*(const float s, const Vector4& v) { return Vector4(v) *= s; }
inline Vector4 operator/(const Vector4& v, const float s) { return Vector4(v) /= s; }
//線形補間
inline Vector4 Lerp(const Vector4& start, const Vector4& end, const float t) 
{ 
	return start + t * (end - start); 
}

//Vector2型変換
inline Vector2::Vector2(const Vector3& v) : x(v.x), y(v.y) {}
//Vector3型変換
inline Vector3::Vector3(const Vector2& v) : x(v.x), y(v.y), z(0) {}
inline Vector3::Vector3(const Vector4& v) : x(v.x), y(v.y), z(v.z) {}
//Vector4型変換
inline Vector4::Vector4(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(0) {}
} // namespace gamelib

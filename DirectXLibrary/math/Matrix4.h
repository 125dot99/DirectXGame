#pragma once
#include "Vector.h"
namespace gamelib
{
//4x4の行列
struct Matrix4
{
	float m[4][4];

	Matrix4() : m() {}
	Matrix4(
		float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23,
		float a30, float a31, float a32, float a33) : m 
	{ 
		a00, a01, a02, a03,
		a10, a11, a12, a13,
		a20, a21, a22, a23,
		a30, a31, a32, a33
	} {}
	//代入演算子オーバーロード
	inline Matrix4& operator*=(const Matrix4& m1)
	{
		Matrix4 result;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					result.m[i][j] += m[i][k] * m1.m[k][j];
				}
			}
		}
		return *this = result;
	}
};

//単位行列
inline Matrix4 MatrixIdentity()
{
	static const Matrix4 IDENTITY(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	return IDENTITY;
}
//拡大縮小行列
inline Matrix4 MatrixScale(const Vector3& s)
{
	return Matrix4(
		s.x, 0.0f, 0.0f, 0.0f,
		0.0f, s.y, 0.0f, 0.0f,
		0.0f, 0.0f, s.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}
//1次元回転行列
//X軸周り
inline Matrix4 MatrixRotateX(const float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);
	return Matrix4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos,  sin,  0.0f,
		0.0f, -sin, cos,  0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}
//Y軸周り
inline Matrix4 MatrixRotateY(const float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);
	return Matrix4(
		cos,  0.0f, -sin, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin,  0.0f, cos,  0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}
//Z軸周り
inline Matrix4 MatrixRotateZ(const float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);
	return Matrix4(
		cos,  sin,  0.0f, 0.0f,
		-sin, cos,  0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}
//平行移動行列
inline Matrix4 MatrixTranslate(const Vector3& t)
{
	return Matrix4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		t.x, t.y, t.z, 1.0f);
}
//座標変換
inline Vector3 Transform(const Vector3& v, const Matrix4& m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
	Vector3 result(
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]);
	return result / w;
}

inline Vector4 Transform(const Vector4& v, const Matrix4& m)
{
	return Vector4(
		v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] +  v.w * m.m[0][3],
		v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] +  v.w * m.m[1][3],
		v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] +  v.w * m.m[2][3],
		v.x * m.m[3][0] + v.y * m.m[3][1] + v.z * m.m[3][2] +  v.w * m.m[3][3]);
}

//左手系ビュー行列を作る
inline Matrix4 MatrixLookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	Vector3 vZ = (target - eye).Normalize(); //Z軸
	Vector3 vX = up.Cross(vZ); //X軸
	Vector3 vY = vZ.Cross(vX); //Y軸
	return Matrix4(
		vX.x, vY.x, vZ.x, 0.0f,
		vX.y, vY.y, vZ.y, 0.0f,
		vX.z, vY.z, vZ.z, 0.0f,
		-eye.Dot(vX), -eye.Dot(vY), -eye.Dot(vZ), 1.0f);
}
//左手系正射影行列を作る
inline Matrix4 MatrixOrthoLH(float width, float height, float nearZ, float farZ)
{
	return Matrix4(
		2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (farZ - nearZ), 0.0f,
		0.0f, 0.0f, -nearZ / (nearZ - farZ), 1.0f);
}
//左手系透視投影行列を作る
inline Matrix4 MatrixPerspectiveFovLH(float fov, float aspect, float nearZ, float farZ) 
{
	float height = 1.0f / tanf(fov * 0.5f);
	float width = height / aspect;
	float depth = farZ - nearZ;
	return Matrix4(
		width, 0.0f, 0.0f, 0.0f,
		0.0f, height, 0.0f, 0.0f,
		0.0f, 0.0f, farZ / depth, 1.0f,
		0.0f, 0.0f, -nearZ * farZ / depth, 0.0f);
}
//平行投影行列を返す
inline Matrix4 MatrixScreen(float width, float height)
{
	return Matrix4(
		2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f);
}
//ビュー行列の転置行列を作る
inline Matrix4 MatrixBillboard(const Matrix4& m, bool isRotY)
{
	Matrix4 billBoard = m;
	billBoard.m[0][1] = m.m[1][0]; //a12 -> A21
	billBoard.m[0][2] = m.m[2][0]; //a13 -> A31
	if (!isRotY)
	{
		billBoard.m[1][0] = m.m[0][1]; //a21 -> A12
		billBoard.m[1][2] = m.m[2][1]; //a23 -> A32
	}
	billBoard.m[2][0] = m.m[0][2]; //a31 -> A13
	billBoard.m[2][1] = m.m[1][2]; //a32 -> A23
	billBoard.m[3][3] = 1.0f;
	return billBoard;
}
//逆行列
inline Matrix4 MatrixInverse(const Matrix4& m)
{
	const float& a00 = m.m[0][0], a01 = m.m[0][1], a02 = m.m[0][2], a03 = m.m[0][3];
	const float& a10 = m.m[1][0], a11 = m.m[1][1], a12 = m.m[1][2], a13 = m.m[1][3];
	const float& a20 = m.m[2][0], a21 = m.m[2][1], a22 = m.m[2][2], a23 = m.m[2][3];
	const float& a30 = m.m[3][0], a31 = m.m[3][1], a32 = m.m[3][2], a33 = m.m[3][3];
	float det = (
		(a00 * a11 * a22 * a33) + (a00 * a12 * a23 * a31) + (a00 * a13 * a21 * a32) -
		(a00 * a13 * a22 * a31) - (a00 * a12 * a21 * a33) - (a00 * a11 * a23 * a32) -
		(a01 * a10 * a22 * a33) - (a02 * a10 * a23 * a32) - (a03 * a10 * a21 * a32) +
		(a03 * a10 * a22 * a31) + (a02 * a10 * a21 * a33) + (a01 * a10 * a23 * a32) +
		(a01 * a12 * a20 * a33) + (a02 * a13 * a20 * a31) + (a03 * a11 * a20 * a32) -
		(a03 * a12 * a20 * a31) - (a02 * a11 * a20 * a33) - (a01 * a13 * a20 * a32) -
		(a01 * a12 * a23 * a30) - (a02 * a13 * a21 * a30) - (a03 * a11 * a22 * a30) -
		(a03 * a12 * a21 * a30) + (a02 * a11 * a23 * a30) + (a01 * a13 * a22 * a30));
	if (det == 0)
	{
		return m;
	}
	Matrix4 result;
	result.m[0][0] = (a11 * a22 * a33 + a12 * a23 * a31 + a13 * a21 * a32 - a11 * a23 * a32 - a12 * a21 * a33 - a13 * a22 * a31);
	result.m[0][1] = (a01 * a23 * a32 + a02 * a21 * a33 + a03 * a22 * a31 - a01 * a22 * a33 - a02 * a23 * a31 - a03 * a21 * a32);
	result.m[0][2] = (a01 * a12 * a33 + a02 * a13 * a31 + a03 * a11 * a32 - a01 * a13 * a32 - a02 * a11 * a33 - a03 * a12 * a31);
	result.m[0][3] = (a01 * a13 * a22 + a02 * a11 * a23 + a03 * a12 * a21 - a01 * a12 * a23 - a02 * a13 * a21 - a03 * a11 * a22);
	result.m[1][0] = (a10 * a23 * a32 + a12 * a20 * a33 + a13 * a22 * a30 - a10 * a22 * a33 - a12 * a23 * a30 - a13 * a20 * a32);
	result.m[1][1] = (a00 * a22 * a33 + a02 * a23 * a30 + a03 * a20 * a32 - a00 * a23 * a32 - a02 * a20 * a33 - a03 * a22 * a30);
	result.m[1][2] = (a00 * a13 * a32 + a02 * a10 * a33 + a03 * a12 * a30 - a00 * a12 * a33 - a02 * a13 * a30 - a03 * a10 * a32);
	result.m[1][3] = (a00 * a12 * a23 + a02 * a13 * a20 + a03 * a10 * a22 - a00 * a13 * a22 - a02 * a10 * a23 - a03 * a12 * a20);
	result.m[2][0] = (a10 * a21 * a33 + a11 * a23 * a30 + a13 * a20 * a31 - a10 * a23 * a31 - a11 * a20 * a33 - a13 * a21 * a30);
	result.m[2][1] = (a00 * a23 * a31 + a01 * a20 * a33 + a03 * a21 * a30 - a00 * a21 * a33 - a01 * a23 * a30 - a03 * a20 * a31);
	result.m[2][2] = (a00 * a11 * a33 + a01 * a13 * a30 + a03 * a10 * a31 - a00 * a13 * a31 - a01 * a10 * a33 - a03 * a11 * a30);
	result.m[2][3] = (a00 * a13 * a21 + a01 * a10 * a23 + a03 * a11 * a20 - a00 * a11 * a23 - a01 * a13 * a20 - a03 * a10 * a21);
	result.m[3][0] = (a10 * a22 * a31 + a11 * a20 * a32 + a12 * a21 * a30 - a10 * a21 * a32 - a11 * a22 * a30 - a12 * a20 * a31);
	result.m[3][1] = (a00 * a21 * a32 + a01 * a22 * a30 + a02 * a20 * a31 - a00 * a22 * a31 - a01 * a20 * a32 - a02 * a21 * a30);
	result.m[3][2] = (a00 * a12 * a31 + a01 * a10 * a32 + a02 * a11 * a30 - a00 * a11 * a32 - a01 * a12 * a30 - a02 * a10 * a31);
	result.m[3][3] = (a00 * a11 * a22 + a01 * a12 * a20 + a02 * a10 * a21 - a00 * a12 * a21 - a01 * a10 * a22 - a02 * a11 * a20);
	det = 1.0f / det;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] *= det;
		}
	}
	return result;
}
//行と列を入れ替える
inline Matrix4 MatrixTranspose(const Matrix4& m)
{
	return Matrix4(
		m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
		m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
		m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
		m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]);
}

inline Vector3 operator*(const Vector3& v, const Matrix4& m) { return Transform(v, m); }
inline Matrix4 operator*(const Matrix4& m1, const Matrix4& m2) { return Matrix4(m1) *= m2; }

} // namespace gamelib
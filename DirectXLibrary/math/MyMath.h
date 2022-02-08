#pragma once
namespace gamelib
{
namespace math
{
static constexpr float PI = 3.141592654f;
static constexpr float PI2 = PI * 2.0f;
static constexpr float RAD = PI / 180.0f;
static constexpr float DEG = 180.0f / PI;
} // namespace math

//線形補間
inline float Lerp(float start, float end, float t)
{
	return start + t * (end - start);
}

//角度をラジアンに変換
inline float ConvertToRadian(float degree)
{
	return degree * math::RAD;
}

//ラジアンを角度に変換
inline float ConvertToDegree(float radian)
{
	return radian * math::DEG;
}

template<typename T>
inline T Sign(const T& x)
{
	return x > 0.0f ? +1.0f : x < 0.0f ? -1.0f : 0.0f;
}

//最小値を返す
template<typename T>
inline T Min(const T& min, const T& x)
{
	return min > x ? min : x;
}

//最大値を返す
template<typename T>
inline T Max(const T& max, const T& x)
{
	return max < x ? max : x;
}

//範囲内に値をクランプする
template<typename T>
inline T Clamp(const T& x, const T& min, const T& max)
{
	return Min(min, Max(max, x));
}

//範囲内で値をラップアラウンドする
template<typename T>
inline T Wrap(const T& x, const T& min, const T& max)
{
	return min > x ? max : max < x ? min : x;
}
} // namespace gamelib
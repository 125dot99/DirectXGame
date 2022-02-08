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

//���`���
inline float Lerp(float start, float end, float t)
{
	return start + t * (end - start);
}

//�p�x�����W�A���ɕϊ�
inline float ConvertToRadian(float degree)
{
	return degree * math::RAD;
}

//���W�A�����p�x�ɕϊ�
inline float ConvertToDegree(float radian)
{
	return radian * math::DEG;
}

template<typename T>
inline T Sign(const T& x)
{
	return x > 0.0f ? +1.0f : x < 0.0f ? -1.0f : 0.0f;
}

//�ŏ��l��Ԃ�
template<typename T>
inline T Min(const T& min, const T& x)
{
	return min > x ? min : x;
}

//�ő�l��Ԃ�
template<typename T>
inline T Max(const T& max, const T& x)
{
	return max < x ? max : x;
}

//�͈͓��ɒl���N�����v����
template<typename T>
inline T Clamp(const T& x, const T& min, const T& max)
{
	return Min(min, Max(max, x));
}

//�͈͓��Œl�����b�v�A���E���h����
template<typename T>
inline T Wrap(const T& x, const T& min, const T& max)
{
	return min > x ? max : max < x ? min : x;
}
} // namespace gamelib
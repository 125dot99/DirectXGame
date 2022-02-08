#pragma once
#include "../math/Vector.h"
#include "../math/MyMath.h"

namespace gamelib
{
namespace color_util
{
//RGBをHSVに変換
inline Vector3 ConvertToHSV(const Vector3& rgb)
{
	Vector3 hsv;
	//RGBの最小値
	float minvalue = Min(rgb.x, Min(rgb.y, rgb.z));
	//RGBの最大値
	float maxvalue = Max(rgb.x, Max(rgb.y, rgb.z));
	//最大値と最小値の差
	float delta = maxvalue - minvalue;
	//V(明度) 一番強い色
	hsv.z = maxvalue;
	//S(彩度) 最大値と最小値の差を正規化して求める
	if (maxvalue != 0.0f)
	{
		hsv.y = delta / maxvalue;
	}
	//H(色相) RGBのうち最大値と最小値の差から求める
	if (hsv.y <= 0.0f)
	{
		return hsv;
	}
	if (rgb.x == maxvalue)
	{
		hsv.x = (rgb.y - rgb.z) / delta;
	}
	else if (rgb.y == maxvalue)
	{
		hsv.x = 2 + (rgb.z - rgb.x) / delta;
	}
	else
	{
		hsv.x = 4 + (rgb.x - rgb.y) / delta;
	}
	hsv.x /= 6.0f;
	if (hsv.x < 0.0f)
	{
		hsv.x += 1.0f;
	}
	return hsv;
}

//HSVをRGBに変換
inline Vector3 ConvertToRGB(const Vector3& hsv)
{
	//S(彩度)
	if (hsv.y == 0)
	{
		return Vector3(hsv.z, hsv.z, hsv.z);
	}
	float i = floorf(hsv.x * 6.0f);
	float f = hsv.x * 6.0f - i;
	float aa = hsv.z * (1.0f - hsv.y);
	float bb = hsv.z * (1.0f - (hsv.y * f));
	float cc = hsv.z * (1.0f - (hsv.y * (1.0f - f)));
	switch ((int)i)
	{
	case 0:
		return Vector3(hsv.z, cc, aa);
	case 1:
		return Vector3(bb, hsv.z, aa);
	case 2:
		return Vector3(aa, hsv.z, cc);
	case 3:
		return Vector3(aa, bb, hsv.z);
	case 4:
		return Vector3(cc, aa, hsv.z);
	default:
		return Vector3(hsv.z, aa, bb);
	}
	return Vector3::Zero();
}
} // namespace color_util
} // namespace gamelib
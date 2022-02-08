#pragma once
#include "../math/Vector.h"
#include "../math/MyMath.h"

namespace gamelib
{
namespace color_util
{
//RGB��HSV�ɕϊ�
inline Vector3 ConvertToHSV(const Vector3& rgb)
{
	Vector3 hsv;
	//RGB�̍ŏ��l
	float minvalue = Min(rgb.x, Min(rgb.y, rgb.z));
	//RGB�̍ő�l
	float maxvalue = Max(rgb.x, Max(rgb.y, rgb.z));
	//�ő�l�ƍŏ��l�̍�
	float delta = maxvalue - minvalue;
	//V(���x) ��ԋ����F
	hsv.z = maxvalue;
	//S(�ʓx) �ő�l�ƍŏ��l�̍��𐳋K�����ċ��߂�
	if (maxvalue != 0.0f)
	{
		hsv.y = delta / maxvalue;
	}
	//H(�F��) RGB�̂����ő�l�ƍŏ��l�̍����狁�߂�
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

//HSV��RGB�ɕϊ�
inline Vector3 ConvertToRGB(const Vector3& hsv)
{
	//S(�ʓx)
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
#include "Easing.h"

#include <cmath>
#include "MyMath.h"

float gamelib::easing::EaseInSine(float x)
{
	return 1.0f - cosf((x * math::PI) / 2.0f);
}

float gamelib::easing::EaseInCubic(float x)
{
	return x * x * x;
}

float gamelib::easing::EaseInQuint(float x)
{
	return x * x * x * x * x;
}

float gamelib::easing::EaseInCirc(float x)
{
	return 1.0f - sqrtf(1.0f - powf(x, 2.0f));
}

float gamelib::easing::EaseInElastic(float x)
{
	const float C4 = math::PI2 / 3.0f;
	if (x == 0.0f)
	{
		return 0.0f;
	}
	else if (x == 1.0f)
	{
		return 1.0f;
	}
	return -powf(2.0f, 10.0f * x - 10.0f) * sinf((x * 10.0f - 10.75f) * C4);
}

float gamelib::easing::EaseInQuad(float x)
{
	return x * x;
}

float gamelib::easing::EaseInQuart(float x)
{
	return x * x * x * x;
}

float gamelib::easing::EaseInExpo(float x)
{
	return x == 0.0f ? 0.0f : powf(2.0f, 10.0f * x - 10.0f);
}

float gamelib::easing::EaseInBack(float x)
{
	static const float C1 = 1.70158f;
	static const float C3 = C1 + 1.0f;
	return C3 * x * x * x - C1 * x * x;
}

float gamelib::easing::EaseInBounce(float x)
{
	return 1.0f - EaseOutBounce(1.0f - x);
}

float gamelib::easing::EaseOutSine(float x)
{
	return sinf((x * math::PI) / 2.0f);
}

float gamelib::easing::EaseOutCubic(float x)
{
	return 1.0f - powf(1.0f - x, 3.0f);
}

float gamelib::easing::EaseOutQuint(float x)
{
	return 1.0f - powf(1.0f - x, 5.0f);
}

float gamelib::easing::EaseOutCirc(float x)
{
	return sqrtf(1.0f - powf(x - 1.0f, 2.0f));
}

float gamelib::easing::EaseOutElastic(float x)
{
	static const float C4 = math::PI2 / 3.0f;
	if (x == 0.0f)
	{
		return 0;
	}
	else if (x == 1.0f)
	{
		return 1.0f;
	}
	return powf(2.0f, -10.0f * x) * sinf((x * 10.0f - 0.75f) * C4) + 1.0f;
}

float gamelib::easing::EaseOutQuad(float x)
{
	return 1.0f - (1.0f - x) * (1.0f - x);
}

float gamelib::easing::EaseOutQuart(float x)
{
	return 1.0f - powf(1.0f - x, 4.0f);
}

float gamelib::easing::EaseOutExpo(float x)
{
	return x == 1.0f ? 1.0f : 1.0f - powf(2.0f, -10.0f * x);
}

float gamelib::easing::EaseOutBack(float x)
{
	static const float C1 = 1.70158f;
	static const float C3 = C1 + 1.0f;
	return 1.0f + C3 * powf(x - 1.0f, 3.0f) + C1 * powf(x - 1.0f, 2.0f);
}

float gamelib::easing::EaseOutBounce(float x)
{
	static const float N1 = 7.5625f;
	static const float D1 = 2.75f;
	if (x < 1.0f / D1)
	{
		return N1 * x * x;
	}
	else if (x < 2.0f / D1)
	{
		return N1 * (x -= 1.5f / D1) * x + 0.75f;
	}
	else if (x < 2.5f / D1)
	{
		return N1 * (x -= 2.25f / D1) * x + 0.9375f;
	}
	return N1 * (x -= 2.625f / D1) * x + 0.984375f;
}

float gamelib::easing::EaseInOutSine(float x)
{
	return -(cosf(math::PI * x) - 1.0f) / 2.0f;
}

float gamelib::easing::EaseInOutCubic(float x)
{
	if (x < 0.5f)
	{
		return 4.0f * x * x * x;
	}
	return 1.0f - powf(-2.0f * x + 2.0f, 3.0f) / 2.0f;
}

float gamelib::easing::EaseInOutQuint(float x)
{
	if (x < 0.5f)
	{
		return 16.0f * x * x * x * x * x;
	}
	return 1.0f - powf(-2.0f * x + 2.0f, 5.0f) / 2.0f;
}

float gamelib::easing::EaseInOutCirc(float x)
{
	if (x < 0.5f)
	{
		return (1.0f - sqrtf(1.0f - powf(2.0f * x, 2.0f))) / 2.0f;
	}
	return (sqrtf(1.0f - powf(-2.0f * x + 2.0f, 2.0f)) + 1.0f) / 2.0f;
}

float gamelib::easing::EaseInOutElastic(float x)
{
	static const float C5 = math::PI2 / 4.5f;
	if (x == 0)
	{
		return 0;
	}
	else if (x == 1)
	{
		return 1.0f;
	}
	else if (x < 0.5f)
	{
		return -(powf(2.0f, 20.0f * x - 10.0f) * sinf((20.0f * x - 11.125f) * C5)) / 2.0f;
	}
	return (powf(2.0f, -20.0f * x + 10.0f) * sinf((20.0f * x - 11.125f) * C5)) / 2.0f + 1.0f;
}

float gamelib::easing::EaseInOutQuad(float x)
{
	if (x < 0.5f)
	{
		return 2.0f * x * x;
	}
	return 1.0f - powf(-2.0f * x + 2.0f, 2.0f) / 2.0f;
}

float gamelib::easing::EaseInOutQuart(float x)
{
	if (x < 0.5f)
	{
		return 8.0f * x * x * x * x;
	}
	return 1.0f - powf(-2.0f * x + 2.0f, 4.0f) / 2.0f;
}

float gamelib::easing::EaseInOutExpo(float x)
{
	if (x == 0)
	{
		return 0;
	}
	else if (x == 1)
	{
		return 1;
	}
	else if (x < 0.5f)
	{
		return powf(2.0f, 20.0f * x - 10.0f) / 2.0f;
	}
	return (2.0f - powf(2.0f, -20.0f * x + 10.0f)) / 2.0f;
}

float gamelib::easing::EaseInOutBack(float x)
{
	static const float C1 = 1.70158f;
	static const float C2 = C1 + 1.525f;
	if (x < 0.5f)
	{
		return (powf(2.0f * x, 2.0f) * ((C2 + 1.0f) * 2.0f * x - C2)) / 2.0f;
	}
	return (powf(2.0f * x - 2.0f, 2.0f) * ((C2 + 1.0f) * (x * 2.0f - 2.0f) + C2) + 2.0f) / 2.0f;
}

float gamelib::easing::EaseInOutBounce(float x)
{
	if (x < 0.5f)
	{
		return (1.0f - EaseOutBounce(1.0f - 2.0f * x)) / 2.0f;
	}
	return (1.0f + EaseOutBounce(2.0f * x - 1.0f)) / 2.0f;
}
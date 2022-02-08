#pragma once
#include "../math/Vector.h"
namespace gamelib
{
struct CameraTransform
{
	Vector3 eye;
	Vector3 target;
	Vector3 up;

	Vector3 Direction() const
	{
		return (target - eye).Normalize();
	}
};
} // namespace gamelib
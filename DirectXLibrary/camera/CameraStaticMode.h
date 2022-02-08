#pragma once
#include "ICameraState.h"
namespace gamelib
{
class CameraStaticMode : public ICameraState
{
public:
	CameraStaticMode() = default;

	inline void Update() override {}
};
} // namespace gamelib

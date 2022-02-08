#pragma once
#include "ICameraState.h"
#include "CameraTransform.h"

namespace gamelib
{
class CameraDebugMode : public ICameraState
{
private:
	Vector3 preEye;
	float distance;
	CameraTransform* owner;
public:
	CameraDebugMode(CameraTransform* _owner);
	void Update() override;
};
} // namespace gamelib
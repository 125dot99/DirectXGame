#pragma once
#include "ICameraRead.h"
#include <memory>

namespace gamelib
{
class SceneCameraProvide
{
private:
	static ICameraRead* p_mainCamera;

	SceneCameraProvide() = delete;
	~SceneCameraProvide() = delete;
public:
	static void SetMainCamera(ICameraRead* p_camera);

	static ICameraRead*& GetCamera();
};
} // namespace gamelib


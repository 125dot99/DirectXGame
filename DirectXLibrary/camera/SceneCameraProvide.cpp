#include "SceneCameraProvide.h"
#include <assert.h>

gamelib::ICameraRead* gamelib::SceneCameraProvide::p_mainCamera;

void gamelib::SceneCameraProvide::SetMainCamera(ICameraRead* p_camera)
{
	if (p_camera == nullptr)
	{
		return;
	}
	p_mainCamera = p_camera;
}

gamelib::ICameraRead*& gamelib::SceneCameraProvide::GetCamera()
{
	assert(p_mainCamera != nullptr && "カメラの参照に失敗しました");
	return p_mainCamera;
}


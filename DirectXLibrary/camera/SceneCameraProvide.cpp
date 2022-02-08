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
	assert(p_mainCamera != nullptr && "ƒJƒƒ‰‚ÌQÆ‚É¸”s‚µ‚Ü‚µ‚½");
	return p_mainCamera;
}


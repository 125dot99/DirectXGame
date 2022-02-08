#pragma once
#include "camera/Camera.h"

using namespace gamelib;

//反射カメラ　水面の映り込み用カメラ
class ReflectCamera : public Camera
{
public:
	ReflectCamera(const Camera& camera);
	void Update() override;
};

#pragma once
#include "camera/Camera.h"

using namespace gamelib;

//���˃J�����@���ʂ̉f�荞�ݗp�J����
class ReflectCamera : public Camera
{
public:
	ReflectCamera(const Camera& camera);
	void Update() override;
};

#pragma once
#include <camera/ICameraState.h>
#include <camera/CameraTransform.h>
#include <object/GameObject.h>

using namespace gamelib;

class CameraTalkMode : public ICameraState
{
private:
	float distance;

	Vector3 endEye;
	Vector3 endTarget;

	CameraTransform* owner;
public:
	CameraTalkMode(CameraTransform* _owner, const Vector3& objectPos1, const Vector3& objectPos2);
	void Update() override;
};

class CameraTargetMode : public ICameraState
{
private:
	GameObject* targetObject;
	CameraTransform* owner;
public:
	CameraTargetMode(CameraTransform* _owner, GameObject* pTargetObject);
	void Update() override;
};
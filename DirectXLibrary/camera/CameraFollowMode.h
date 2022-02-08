#pragma once
#include "ICameraState.h"
#include "CameraTransform.h"
#include "../object/GameObject.h"
namespace gamelib
{
//�Ǐ]����J����
class CameraFollowMode : public ICameraState
{
private:
	float distance;
	Vector3 offset;
	CameraTransform* owner;
	GameObject* targetObject;
public:
	CameraFollowMode(CameraTransform* _owner, GameObject* pTargetObject);
	void Update() override;
};
} // namespace gamelib
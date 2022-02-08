#include "GameCameraControl.h"

CameraTalkMode::CameraTalkMode(CameraTransform* _owner, const Vector3& objectPos1, const Vector3& objectPos2) : owner(_owner)
{
	Vector3 offset = Vector3(0, 0, 0);

	distance = (owner->target - owner->eye).SqrtLength();
	//endTarget = objectPos1 + offset + ((objectPos1 + objectPos2) * 0.5f);
	endTarget = offset + objectPos2;
}

void CameraTalkMode::Update()
{
	owner->target = Lerp(owner->target, endTarget, 0.1f);
	//ƒJƒƒ‰‚ª‰ñ‚è‚Ş‚æ‚¤‚ÉˆÚ“®
	Vector3 forward = owner->Direction();
	owner->eye = owner->target + forward * -distance;
}

CameraTargetMode::CameraTargetMode(CameraTransform* _owner, GameObject* pTargetObject) : owner(_owner), targetObject(pTargetObject)
{
	owner->eye = Vector3(0, 2.5f, -3.5f);
}

void CameraTargetMode::Update()
{
	owner->target = Lerp(owner->target, targetObject->GetPosition() * Vector3(0.25f, 0, 0.25f), 0.1f);
	owner->target.y = owner->eye.y - 0.5f;
}
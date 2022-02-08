#include "CameraFollowMode.h"

#include "../input/Input.h"
#include "../math/Quaternion.h"

gamelib::CameraFollowMode::CameraFollowMode(CameraTransform* _owner, GameObject* pTargetObject) : owner(_owner), targetObject(pTargetObject)
{
	Vector3 targetPos = targetObject->GetPosition();
	offset = Vector3(0, 3, 0);
	distance = 6.0f;
}

void gamelib::CameraFollowMode::Update()
{
	//’Ç]‚·‚é
	owner->target = Lerp(owner->target, offset + targetObject->GetPosition(), 0.2f);
	
	//ƒJƒƒ‰‚ª‰ñ‚èž‚Þ‚æ‚¤‚ÉˆÚ“®
	Vector3 forward = owner->Direction();
	owner->eye = owner->target + forward * -distance;

	Vector2 rightThumb = Input::GetGamepad()->GetRightThumb();
	Vector2 rot;
	if (Input::GetKeyborad()->IsKey(KEY_CODE::RIGHT) ||
		rightThumb.x > 0)
	{
		rot.x += 1;
	}
	if (Input::GetKeyborad()->IsKey(KEY_CODE::LEFT) ||
		rightThumb.x < 0)
	{
		rot.x -= 1;
	}
	if (Input::GetKeyborad()->IsKey(KEY_CODE::UP) ||
		rightThumb.y > 0)
	{
		rot.y += 1;
	}
	if (Input::GetKeyborad()->IsKey(KEY_CODE::DOWN) ||
		rightThumb.y < 0)
	{
		rot.y -= 1;
	}
	const float R_SPEED = 0.02f;
	Vector3 right = forward.Cross(Vector3::Up());
	right.y = 0;
	Vector3 offset = forward * -distance;
	Quaternion q1 = AngleAxis(Vector3::Up(), rot.x * R_SPEED);
	Quaternion q2 = AngleAxis(right, rot.y * R_SPEED);
	Quaternion q = Quaternion(offset, q1 * q2);
	owner->eye = owner->target + q.Euler();
	owner->eye.y = Min(1.0f, Max(distance, owner->eye.y));
}

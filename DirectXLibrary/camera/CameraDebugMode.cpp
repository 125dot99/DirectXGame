#include "CameraDebugMode.h"

#include "../input/Input.h"
#include "../math/MyMath.h"
#include "../math/Quaternion.h"

gamelib::CameraDebugMode::CameraDebugMode(CameraTransform* _owner) : owner(_owner)
{
	distance = (owner->target - owner->eye).SqrtLength();
	owner->up = Vector3::Up();
}

void gamelib::CameraDebugMode::Update()
{
	auto& leftThomb = Input::GetGamepad()->GetLeftThumb();
	Vector3 v;
	if (Input::GetKeyborad()->IsKey(KEY_CODE::D) ||
		leftThomb.x > 0)
	{
		v.x += 1;
	}
	if (Input::GetKeyborad()->IsKey(KEY_CODE::A) ||
		leftThomb.x < 0)
	{
		v.x -= 1;
	}
	if (Input::GetKeyborad()->IsKey(KEY_CODE::W) ||
		leftThomb.y > 0)
	{
		v.y += 1;
	}
	if (Input::GetKeyborad()->IsKey(KEY_CODE::S) ||
		leftThomb.y < 0)
	{
		v.y -= 1;
	}
	if (Input::GetKeyborad()->IsKey(KEY_CODE::E) ||
		Input::GetGamepad()->IsButton(GAMEPAD_CODE::RIGHT_TRIGGER))
	{
		v.z += 1;
	}
	if (Input::GetKeyborad()->IsKey(KEY_CODE::Z) ||
		Input::GetGamepad()->IsButton(GAMEPAD_CODE::LEFT_TRIGGER))
	{
		v.z -= 1;
	}
	const float MOVE_SPEED = 0.05f;
	Vector3 forwardVec = owner->Direction();
	Vector3 rightVec = Vector3::Up().Cross(forwardVec);
	Vector3 upVec = forwardVec.Cross(rightVec);
	Vector3 mouseVel = (rightVec * v.x + upVec * v.y + forwardVec * v.z) * MOVE_SPEED;
	if (v.Length() > 0)
	{
		owner->eye += mouseVel;
		owner->target += mouseVel;
	}
	auto& rightThomb = Input::GetGamepad()->GetRightThumb();
	Vector2 rot;
	if (Input::GetKeyborad()->IsKey(KEY_CODE::RIGHT) ||
		rightThomb.x > 0)
	{
		rot.x += 1;
	}
	if (Input::GetKeyborad()->IsKey(KEY_CODE::LEFT) ||
		rightThomb.x < 0)
	{
		rot.x -= 1;
	}
	if (Input::GetKeyborad()->IsKey(KEY_CODE::UP) ||
		rightThomb.y > 0)
	{
		rot.y += 1;
	}
	if (Input::GetKeyborad()->IsKey(KEY_CODE::DOWN) ||
		rightThomb.y < 0)
	{
		rot.y -= 1;
	}
	const float R_SPEED = 0.02f;
	rightVec.y = 0;
	Vector3 offset = forwardVec * -distance;
	Quaternion q1 = AngleAxis(Vector3::Up(), rot.x * R_SPEED);
	Quaternion q2 = AngleAxis(rightVec, rot.y * R_SPEED);
	Quaternion q = Quaternion(offset, q1 * q2);
	owner->eye = owner->target + q.Euler();
	owner->eye.y = Clamp(owner->eye.y, 1.0f, distance);
	owner->target.y = Clamp(owner->target.y, 1.0f, distance);
}
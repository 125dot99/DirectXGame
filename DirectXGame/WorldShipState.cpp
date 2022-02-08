#include "WorldShipState.h"

#include "camera/SceneCameraProvide.h"
#include "input/Input.h"
#include "imgui/imgui.h"

WorldShipWait::WorldShipWait(WorldShip* _owner) : owner(_owner)
{
	owner->accel = 0;
	//owner->anima->Play(0);
}

void WorldShipWait::Update()
{

}

WorldShipMove::WorldShipMove(WorldShip* _owner) : owner(_owner)
{

}

void WorldShipMove::Update()
{
	auto& leftThomb = Input::GetGamepad()->GetLeftThumb();
	Vector2 v;
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
	//ZŽ²
	Vector3 forward = SceneCameraProvide::GetCamera()->GetDirection();
	//XŽ²
	Vector3 right = Vector3::Up().Cross(forward);
	owner->accel = v.x * right + v.y * forward;
	owner->accel.y = 0;
	owner->accel *= owner->speed;
	//‰ñ“]
	Vector2 rot(owner->velocity.x, owner->velocity.z);
	//rot *= 1.0f - owner->RESISTANCE;
	rot = rot.Normalize();
	float y = asinf(rot.x);
	if (rot.y < 0)
	{
		y = math::PI - y;
	}
	owner->rotation.y = ConvertToDegree(y);
}

WorldShipTalk::WorldShipTalk(WorldShip* _owner) : owner(_owner)
{

}

void WorldShipTalk::Update()
{
}


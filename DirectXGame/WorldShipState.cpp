#include "WorldShipState.h"

#include "camera/SceneCameraProvide.h"
#include "InputControl.h"

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
	const Vector2 v = input_control::LeftStickValue();
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


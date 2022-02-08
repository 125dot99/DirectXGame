#include "WorldShip.h"

#include "input/Input.h"
#include "GameWorld.h"
#include "WorldShipState.h"
#include "collision/CollisionHelper.h"

WorldShip::WorldShip(FbxAnimation* anima) : anima(anima)
{
	//èâä˙âªéû
	state_enum = STATE_ENUM::WAIT;
	state = std::make_unique<WorldShipWait>(this);
	anima->Play(1);
}

void WorldShip::Initialize()
{
	position = Vector3(0, -0.5f, -20.0f);
	speed = 0.1f;
	scale = 1.2f;
}

void WorldShip::Update()
{
	switch (state_enum)
	{
	case STATE_ENUM::WAIT:
		if (Input::GetKeyborad()->IsKey(KEY_CODE::D) ||
			Input::GetKeyborad()->IsKey(KEY_CODE::A) ||
			Input::GetKeyborad()->IsKey(KEY_CODE::W) ||
			Input::GetKeyborad()->IsKey(KEY_CODE::S) ||
			Input::GetGamepad()->GetLeftThumb().Length() > 0)
		{
			state_enum = STATE_ENUM::MOVE;
			state = std::make_unique<WorldShipMove>(this);
		}
		break;
	case STATE_ENUM::MOVE:
		if (!(Input::GetKeyborad()->IsKey(KEY_CODE::D) ||
			Input::GetKeyborad()->IsKey(KEY_CODE::A) ||
			Input::GetKeyborad()->IsKey(KEY_CODE::W) ||
			Input::GetKeyborad()->IsKey(KEY_CODE::S) ||
			Input::GetGamepad()->GetLeftThumb().Length() > 0))
		{
			state_enum = STATE_ENUM::WAIT;
			state = std::make_unique<WorldShipWait>(this);
		}
		break;
	case STATE_ENUM::TALK:
		break;
	default:
		break;
	}
	state->Update();
	velocity += accel;
	velocity *= RESISTANCE;
	velocity.y = 0;
	position += velocity * speed;
	position += CollisionHelper::PushInPointRangeCircle(position, game_world::MAP_CIRCLE_WALL_RADIAS);
	position += CollisionHelper::PushOutPointRangeRect(position, game_world::BOX_OUT_CAFE);
	GameObject::Update();
}

void WorldShip::OnCollision(BaseCollider* collA, BaseCollider* collB)
{
	if (collA->GetType() == PRIMITIVE_TYPE::SPHERE &&
		collB->GetType() == PRIMITIVE_TYPE::SPHERE)
	{
		auto v = CollisionHelper::PushSphereHitSphere(collA, collB);
		position += Vector3(v.x, 0, v.z);
	}
}


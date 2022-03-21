#include "WorldShip.h"

#include "InputControl.h"
#include "GameWorld.h"
#include "WorldShipState.h"
#include "collision/CollisionHelper.h"

WorldShip::WorldShip(std::shared_ptr<FbxAnimation> s_pFbxAnimation) : s_pFbxAnimation(s_pFbxAnimation)
{
	//èâä˙âªéû
	state_enum = STATE_ENUM::WAIT;
	state = std::make_unique<WorldShipWait>(this);
	this->s_pFbxAnimation->Play(1);
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
		if (input_control::MoveAction())
		{
			state_enum = STATE_ENUM::MOVE;
			state = std::make_unique<WorldShipMove>(this);
		}
		break;
	case STATE_ENUM::MOVE:
		if (!input_control::MoveAction())
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
	s_pFbxAnimation->Update();
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


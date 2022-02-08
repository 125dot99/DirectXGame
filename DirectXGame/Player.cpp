#include "Player.h"

#include "input/Input.h"
#include "GameWorld.h"
#include "PlayerState.h"
#include "collision/CollisionHelper.h"

bool Player::IsMove()
{
	return Input::GetKeyborad()->IsKey(KEY_CODE::D) ||
		Input::GetKeyborad()->IsKey(KEY_CODE::A) ||
		Input::GetKeyborad()->IsKey(KEY_CODE::W) ||
		Input::GetKeyborad()->IsKey(KEY_CODE::S) ||
		Input::GetGamepad()->GetLeftThumb().Length() > 0;
}

Player::Player(FbxAnimation* anima) : anima(anima)
{
	//èâä˙âªéû
	state_enum = STATE_ENUM::WAIT;
	state = std::make_unique<PlayerWait>(this);
}

void Player::Initialize()
{
	rotation.y = 180.0f;
	position = Vector3::Zero();
	speed = 0.02f;
}

void Player::Update()
{
	switch (state_enum)
	{
	case Player::STATE_ENUM::WAIT:
		if (IsMove())
		{
			state_enum = STATE_ENUM::MOVE;
			state = std::make_unique<PlayerMove>(this);
		}
		break;
	case Player::STATE_ENUM::MOVE:
		if (!IsMove())
		{
			state_enum = STATE_ENUM::WAIT;
			state = std::make_unique<PlayerWait>(this);
		}
		break;
	case Player::STATE_ENUM::TALK:
		break;
	default:
		break;
	}
	state->Update();
	GameObject::Update();
}

void Player::OnCollision(BaseCollider* collA, BaseCollider* collB)
{
	if (collA->GetType() == PRIMITIVE_TYPE::SPHERE &&
		collB->GetType() == PRIMITIVE_TYPE::SPHERE)
	{
		auto v = CollisionHelper::PushSphereHitSphere(collA, collB);
		v.y = 0;
		position += v;
	}
}


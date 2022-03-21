#include "Player.h"

#include "input/Input.h"
#include "InputControl.h"
#include "GameWorld.h"
#include "PlayerState.h"
#include "collision/CollisionHelper.h"

Player::Player(std::shared_ptr<FbxAnimation> s_pFbxAnimation, std::weak_ptr<TalkObserver> w_p_talkObserver) : 
	s_pFbxAnimation(s_pFbxAnimation), w_p_talkObserver(w_p_talkObserver)
{
	state_enum = STATE_ENUM::WAIT;
	state = std::make_unique<PlayerWait>(this);
}

void Player::Initialize()
{
	rotation.y = 180.0f;
	position = Vector3(0, 0, -1);
	speed = 0.02f;
}

void Player::Update()
{
	switch (state_enum)
	{
	case Player::STATE_ENUM::WAIT:
		if (input_control::MoveAction() && !pParentObject)
		{
			state_enum = STATE_ENUM::MOVE;
			state = std::make_unique<PlayerMove>(this);
		}
		else if (w_p_talkObserver.lock()->IsTalk())
		{
			state_enum = STATE_ENUM::TALK;
			state = std::make_unique<PlayerTalk>(this);
		}
		break;
	case Player::STATE_ENUM::MOVE:
		if (!input_control::MoveAction())
		{
			state_enum = STATE_ENUM::WAIT;
			state = std::make_unique<PlayerWait>(this);
		}
		else if (w_p_talkObserver.lock()->IsTalk())
		{
			state_enum = STATE_ENUM::TALK;
			state = std::make_unique<PlayerTalk>(this);
		}
		break;
	case Player::STATE_ENUM::TALK:
		if (!w_p_talkObserver.lock()->IsTalk())
		{
			state_enum = STATE_ENUM::WAIT;
			state = std::make_unique<PlayerWait>(this);
		}
		break;
	default:
		state_enum = STATE_ENUM::WAIT;
		state = std::make_unique<PlayerWait>(this);
		break;
	}
	state->Update();
	s_pFbxAnimation->Update();
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


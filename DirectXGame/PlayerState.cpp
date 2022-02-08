#include "PlayerState.h"

#include "input/Input.h"
#include "imgui/imgui.h"

#include "GameWorld.h"
#include "collision/CollisionHelper.h"

PlayerWait::PlayerWait(Player* _owner) : owner(_owner)
{
	owner->accel = 0;
	owner->velocity = 0;
	owner->anima->Play(3);
}

void PlayerWait::Update()
{

}

PlayerMove::PlayerMove(Player* _owner) : owner(_owner)
{
	owner->anima->Play(0);
}

void PlayerMove::Update()
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
	owner->velocity += Vector3(v.x, 0, v.y);
	owner->velocity *= 0.9f;
	owner->position += owner->velocity * owner->speed;

	//‰ñ“]
	Vector2 rot = Vector2(owner->velocity.x, owner->velocity.z).Normalize();
	float y = asinf(rot.x);
	if (rot.y < 0)
	{
		y = math::PI - y;
	}
	owner->velocity.x = Clamp(owner->velocity.x, -1.0f, 1.0f);
	owner->velocity.z = Clamp(owner->velocity.z, -1.0f, 1.0f);

	owner->rotation.y = ConvertToDegree(y) + 180;

	//“X“à‚Ì‹éŒ`”ÍˆÍ
	owner->position += CollisionHelper::PushInPointRangeRect(owner->position, game_world::BOX_IN_CAFE);
	owner->position += CollisionHelper::PushOutPointRangeRect(owner->position, game_world::BOX_IN_CAFE_TABLE_FRONT);
	owner->position += CollisionHelper::PushOutPointRangeRect(owner->position, game_world::BOX_IN_CAFE_TABLE_BACK);
}

PlayerTalk::PlayerTalk(Player* _owner) : owner(_owner)
{

}

void PlayerTalk::Update()
{
}

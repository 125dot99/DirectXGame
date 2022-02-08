#include "MiniPlayer.h"

#include "input/Input.h"
#include "MiniScreenData.h"

void MiniPlayer::Move()
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
	const float SPEED = 3.0f;
	position.x = Clamp(position.x + v.x * SPEED, mini_screen::MIN_WALL.x, mini_screen::MAX_WALL.x);
}

void MiniPlayer::Shot()
{
	if (!Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::B) &&
		!Input::GetKeyborad()->IsKeyDown(KEY_CODE::SPACE))
	{
		return;
	}
	if (bullet->IsShot())
	{
		Vector3 offset(0, -4.0f, 0);
		bullet->SetPosition(position + offset);
		bullet->Shot(Vector2(0, -8.0f));
	}
}

MiniPlayer::MiniPlayer(GameObject* pBullet, HitPoint& hp) : hp(hp)
{
	bullet = dynamic_cast<Bullet*>(pBullet);
}

void MiniPlayer::Initialize()
{
	position = Vector2(mini_screen::MIN_WALL.x, 580);
}

void MiniPlayer::Update()
{
	Move();
	Shot();
	GameObject::UpdateCollider();
}

void MiniPlayer::OnCollision(BaseCollider* collA, BaseCollider* collB)
{
	hp.Damage(1.0f);
}


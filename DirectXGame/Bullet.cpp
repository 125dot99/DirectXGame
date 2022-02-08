#include "Bullet.h"

#include "math/MyMath.h"
#include "MiniScreenData.h"

//const float MAX_Y = -20.0f;

void Bullet::Initialize()
{
	position.y = mini_screen::MIN_WALL.y;
}

void Bullet::Update()
{
	position += velocity;

	if (position.y >= mini_screen::MAX_WALL.y || 
		position.y <= mini_screen::MIN_WALL.y)
	{
		position.y = 0;
	}
	GameObject::UpdateCollider();
}

void Bullet::OnCollision(BaseCollider* collA, BaseCollider* collB)
{
	position.y = 0;
	velocity = 0;
}

void Bullet::Shot(const Vector2& velocity)
{
	this->velocity = velocity;
}

bool Bullet::IsShot()
{
	return position.y <= mini_screen::MIN_WALL.y || position.y >= mini_screen::MAX_WALL.y;
}

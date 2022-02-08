#include "MiniEnemy.h"

#include "MiniScreenData.h"
#include "EnemyManager.h"
#include "Score.h"

const float DEAD_POS_Y = -1000.0f;

MiniEnemy::MiniEnemy(EnemyManager* pEnemyManager, HitPoint& hp, int score) : manager(pEnemyManager), hp(hp), score(score)
{

}

void MiniEnemy::Initialize()
{

}

void MiniEnemy::Update()
{
	if (position.y == DEAD_POS_Y)
	{
		return;
	}
	GameObject::UpdateCollider();
}

void MiniEnemy::OnCollision(BaseCollider* collA, BaseCollider* collB)
{
	Score::Add(score);
	score = 0;
	hp.Damage(1);
	position.x = mini_screen::CENTER.x;
	position.y = DEAD_POS_Y;
	GameObject::UpdateCollider();
}

void MiniEnemy::Move(const Vector2& velocity)
{
	if (position.y == DEAD_POS_Y)
	{
		return;
	}
	position += velocity;
}


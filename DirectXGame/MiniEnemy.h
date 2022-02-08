#pragma once
#include "Bullet.h"
#include "gametemp/HitPoint.h"

using namespace gamelib;

class MiniEnemy : public GameObject
{
private:
	class EnemyManager* manager;
	HitPoint& hp;
	int score;
public:
	MiniEnemy(class EnemyManager* pEnemyManager, HitPoint& hp, int score);

	void Initialize() override;
	void Update() override;
	void OnCollision(BaseCollider* collA, BaseCollider* collB) override;

	void Move(const Vector2& velocity);
	const bool IsDead() const
	{
		const float DEAD_POS_Y = -1000.0f;
		return position.y == DEAD_POS_Y;
	}
};

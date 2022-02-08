#pragma once
#include "object/GameObject.h"

#include "Bullet.h"
#include "gametemp/HitPoint.h"

using namespace gamelib;
class MiniPlayer : public GameObject
{
private:
	HitPoint& hp;
	Bullet* bullet;
	void Move();
	void Shot();
public:
	MiniPlayer(GameObject* pBullet, HitPoint& hp);

	void Initialize() override;
	void Update() override;
	void OnCollision(BaseCollider* collA, BaseCollider* collB) override;
};


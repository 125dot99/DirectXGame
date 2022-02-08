#pragma once
#include "object/GameObject.h"
#include "gametemp/HitPoint.h"

using namespace gamelib;

class MiniPlayerWall : public GameObject
{
private:
	HitPoint& hp;
public:
	MiniPlayerWall(HitPoint& hp);
	void Initialize() override;
	void Update() override;
	void OnCollision(BaseCollider* collA, BaseCollider* collB) override;
};
#pragma once
#include "object/GameObject.h"

using namespace gamelib;
class Bullet : public GameObject
{
private:
	Vector2 velocity;

public:
	void Initialize() override;
	void Update() override;
	void OnCollision(BaseCollider* collA, BaseCollider* collB) override;

	virtual void Shot(const Vector2& velocity);
	virtual bool IsShot();
};


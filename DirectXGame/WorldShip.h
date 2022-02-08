#pragma once
#include "object/GameObject.h"
#include "renderer/FbxAnimation.h"
#include "gametemp/IObjectState.h"

using namespace gamelib;
class WorldShip : public GameObject
{
private:
	enum class STATE_ENUM { WAIT, MOVE, TALK };
	friend class WorldShipWait;
	friend class WorldShipMove;
	friend class WorldShipTalk;
	STATE_ENUM state_enum;

	const float RESISTANCE = 0.95f;
	float speed;
	Vector3 velocity;
	Vector3 accel;

	FbxAnimation* anima;
	std::unique_ptr<IObjectState> state;
public:
	WorldShip(FbxAnimation* anima);
	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//衝突判定
	void OnCollision(BaseCollider* collA, BaseCollider* collB);
};

#pragma once
#include "object/GameObject.h"
#include "renderer/FbxAnimation.h"
#include "gametemp/IObjectState.h"

using namespace gamelib;
class Player : public GameObject
{
private:
	enum class STATE_ENUM { WAIT, MOVE, TALK };
	friend class PlayerWait;
	friend class PlayerMove;
	friend class PlayerTalk;
	STATE_ENUM state_enum;

	float speed;
	Vector3 accel;
	Vector3 velocity;

	FbxAnimation* anima;
	std::unique_ptr<IObjectState> state;
	bool IsMove();
public:
	Player(FbxAnimation* anima);
	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//衝突判定
	void OnCollision(BaseCollider* collA, BaseCollider* collB);
};

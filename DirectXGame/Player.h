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
	//èâä˙âª
	void Initialize() override;
	//çXêV
	void Update() override;
	//è’ìÀîªíË
	void OnCollision(BaseCollider* collA, BaseCollider* collB);
};

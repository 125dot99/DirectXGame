#pragma once
#include "object/GameObject.h"
#include "renderer/FbxAnimation.h"
#include "gametemp/IObjectState.h"
#include "TalkObserver.h"

using namespace gamelib;
class Player : public GameObject
{
private:
	friend class PlayerWait;
	friend class PlayerMove;
	friend class PlayerTalk;
	enum class STATE_ENUM { WAIT, MOVE, TALK };
	STATE_ENUM state_enum;
	std::unique_ptr<IObjectState> state;

	float speed;
	Vector3 accel;
	Vector3 velocity;

	std::shared_ptr<FbxAnimation> s_pFbxAnimation;
	std::weak_ptr<TalkObserver> w_p_talkObserver;
public:
	Player(std::shared_ptr<FbxAnimation> s_pFbxAnimation, std::weak_ptr<TalkObserver> w_p_talkObserver);
	//èâä˙âª
	void Initialize() override;
	//çXêV
	void Update() override;
	//è’ìÀîªíË
	void OnCollision(BaseCollider* collA, BaseCollider* collB);
};

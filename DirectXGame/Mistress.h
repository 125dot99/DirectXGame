#pragma once
#include "object/GameObject.h"
#include "renderer/FbxAnimation.h"
#include "gametemp/IObjectState.h"
#include "TalkObserver.h"

using namespace gamelib;
class Mistress : public GameObject
{
private:
	enum class STATE_ENUM { WAIT, TALK };
	STATE_ENUM state_enum;
	std::unique_ptr<IObjectState> u_pState;

	std::shared_ptr<FbxAnimation> s_pFbxAnimation;
	std::weak_ptr<TalkObserver> w_pTalkObserver;
public:
	Mistress(std::shared_ptr<FbxAnimation> s_pFbxAnimation, std::weak_ptr<TalkObserver> w_pTalkObserver);
	//������
	void Initialize() override;
	//�X�V
	void Update() override;
	//�Փ˔���
	void OnCollision(BaseCollider* collA, BaseCollider* collB);
};


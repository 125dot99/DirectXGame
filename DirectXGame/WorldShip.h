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
	std::unique_ptr<IObjectState> state;

	const float RESISTANCE = 0.95f;
	float speed;
	Vector3 velocity;
	Vector3 accel;

	std::shared_ptr<FbxAnimation> s_pFbxAnimation;
public:
	WorldShip(std::shared_ptr<FbxAnimation> s_pFbxAnimation);
	//èâä˙âª
	void Initialize() override;
	//çXêV
	void Update() override;
	//è’ìÀîªíË
	void OnCollision(BaseCollider* collA, BaseCollider* collB);
};

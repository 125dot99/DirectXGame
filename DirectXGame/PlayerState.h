#pragma once
#include "gametemp/IObjectState.h"
#include "Player.h"

//待機
class PlayerWait : public gamelib::IObjectState
{
private:
	Player* owner;
public:
	PlayerWait(Player* _owner);
	void Update() override;
};

//歩行
class PlayerMove : public gamelib::IObjectState
{
private:
	Player* owner;
public:
	PlayerMove(Player* _owner);
	void Update() override;
};

//会話
class PlayerTalk : public gamelib::IObjectState
{
private:
	Player* owner;
public:
	PlayerTalk(Player* _owner);
	void Update() override;
};
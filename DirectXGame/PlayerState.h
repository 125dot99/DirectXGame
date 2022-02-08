#pragma once
#include "gametemp/IObjectState.h"
#include "Player.h"

//ë“ã@
class PlayerWait : public gamelib::IObjectState
{
private:
	Player* owner;
public:
	PlayerWait(Player* _owner);
	void Update() override;
};

//ï‡çs
class PlayerMove : public gamelib::IObjectState
{
private:
	Player* owner;
public:
	PlayerMove(Player* _owner);
	void Update() override;
};

//âÔòb
class PlayerTalk : public gamelib::IObjectState
{
private:
	Player* owner;
public:
	PlayerTalk(Player* _owner);
	void Update() override;
};
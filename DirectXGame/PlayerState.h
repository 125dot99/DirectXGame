#pragma once
#include "gametemp/IObjectState.h"
#include "Player.h"

//�ҋ@
class PlayerWait : public gamelib::IObjectState
{
private:
	Player* owner;
public:
	PlayerWait(Player* _owner);
	void Update() override;
};

//���s
class PlayerMove : public gamelib::IObjectState
{
private:
	Player* owner;
public:
	PlayerMove(Player* _owner);
	void Update() override;
};

//��b
class PlayerTalk : public gamelib::IObjectState
{
private:
	Player* owner;
public:
	PlayerTalk(Player* _owner);
	void Update() override;
};
#pragma once
#include "gametemp/IObjectState.h"
#include "WorldShip.h"

//�ҋ@
class WorldShipWait : public gamelib::IObjectState
{
private:
	WorldShip* owner;
public:
	WorldShipWait(WorldShip* _owner);
	void Update() override;
};

//���s
class WorldShipMove : public gamelib::IObjectState
{
private:
	WorldShip* owner;
public:
	WorldShipMove(WorldShip* _owner);
	void Update() override;
};

//��b
class WorldShipTalk : public gamelib::IObjectState
{
private:
	WorldShip* owner;
public:
	WorldShipTalk(WorldShip* _owner);
	void Update() override;
};
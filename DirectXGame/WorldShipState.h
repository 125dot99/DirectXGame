#pragma once
#include "gametemp/IObjectState.h"
#include "WorldShip.h"

//待機
class WorldShipWait : public gamelib::IObjectState
{
private:
	WorldShip* owner;
public:
	WorldShipWait(WorldShip* _owner);
	void Update() override;
};

//歩行
class WorldShipMove : public gamelib::IObjectState
{
private:
	WorldShip* owner;
public:
	WorldShipMove(WorldShip* _owner);
	void Update() override;
};

//会話
class WorldShipTalk : public gamelib::IObjectState
{
private:
	WorldShip* owner;
public:
	WorldShipTalk(WorldShip* _owner);
	void Update() override;
};
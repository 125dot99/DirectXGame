#pragma once
#include "gametemp/IObjectState.h"
#include "WorldShip.h"

//ë“ã@
class WorldShipWait : public gamelib::IObjectState
{
private:
	WorldShip* owner;
public:
	WorldShipWait(WorldShip* _owner);
	void Update() override;
};

//ï‡çs
class WorldShipMove : public gamelib::IObjectState
{
private:
	WorldShip* owner;
public:
	WorldShipMove(WorldShip* _owner);
	void Update() override;
};

//âÔòb
class WorldShipTalk : public gamelib::IObjectState
{
private:
	WorldShip* owner;
public:
	WorldShipTalk(WorldShip* _owner);
	void Update() override;
};
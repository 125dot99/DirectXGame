#pragma once
#include "light/LightGroup.h"

using namespace gamelib;
class GameClock
{
private:
	static DirectionalLight sunLight;
	static DirectionalLight moonLight;

	static LightGroup* llghtGroup;

	GameClock() = delete;
	~GameClock() = delete;
public:
	static void Initialize();

	static void SetLightGroup(LightGroup* pLlghtGroup);
	
	static void Update();

};


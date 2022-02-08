#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "Light.h"
#include "SkyFromAtmosphere.h"

using namespace gamelib;
namespace scene
{
class SkyScene : public BaseScene
{
private:
	//ƒƒ“ƒo•Ï”éŒ¾
	Camera* camera = nullptr;
	DirectionalLight* light = nullptr;
	
	SkyFromAtmosphere* skyObject = nullptr;
	//Œp³ƒƒ“ƒoŠÖ”
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void NextScene(SceneManager* pScene) override;
};
}


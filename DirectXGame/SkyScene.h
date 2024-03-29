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
	//メンバ変数宣言
	Camera* camera = nullptr;
	DirectionalLight* light = nullptr;
	
	SkyFromAtmosphere* skyObject = nullptr;
	//継承メンバ関数
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void NextScene(SceneManager* pScene) override;
};
}


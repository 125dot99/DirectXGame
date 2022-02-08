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
	//�����o�ϐ��錾
	Camera* camera = nullptr;
	DirectionalLight* light = nullptr;
	
	SkyFromAtmosphere* skyObject = nullptr;
	//�p�������o�֐�
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void NextScene(SceneManager* pScene) override;
};
}


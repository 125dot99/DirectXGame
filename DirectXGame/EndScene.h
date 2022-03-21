#pragma once
#include <scene\BaseScene.h>

using namespace gamelib;

class EndScene : public BaseScene
{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void NextScene(ISceneManager* pSceneManager) override;
};


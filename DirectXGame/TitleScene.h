#pragma once
#include <GameLibHeader.h>

using namespace gamelib;

class TitleScene : public BaseScene
{
private:
	std::vector<std::weak_ptr<Texture>> textures;
	std::unique_ptr<SpriteRenderer> spriteRenderer;
	std::unique_ptr<BaseChildScene> fadeScene;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void NextScene(ISceneManager* pSceneManager) override;
};

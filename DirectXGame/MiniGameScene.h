#pragma once
#include <GameLibHeader.h>

#include "gametemp/HitPoint.h"
#include "EnemyManager.h"

using namespace gamelib;

class MiniGameScene : public BaseScene
{
private:
	friend class MiniTitle;
	friend class MiniPlay;
	friend class MiniGameClear;
	friend class MiniGameOver;
	
	std::string strHiScore;
	float animaFrame;

	Vector2 windowSize;

	HitPoint playerHp;
	HitPoint enemyHp;

	std::unique_ptr<BaseChildScene> childScene;
	std::vector<std::weak_ptr<Texture>> textures;
	std::unique_ptr<SpriteRenderer> spriteRenderer;
	std::unique_ptr<SpriteFontAscii> spriteFontAscii;
	std::unique_ptr<IPostProcess> filter;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void NextScene(ISceneManager* pSceneManager) override;
};

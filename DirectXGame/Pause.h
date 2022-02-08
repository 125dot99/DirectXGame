#pragma once
#include <GameLibHeader.h>
#include "SimpleFilter.h"

//ポーズ時の表示物、入力を管理するクラス
class Pause : public BaseChildScene
{
private:
	SpriteAnimation anima;
	SpriteRenderer* spriteRenderer;

	std::unique_ptr<SimpleFilter> simpleFilter;
	std::vector<std::weak_ptr<Texture>> textures;
	std::unique_ptr<SpriteFont> spriteFont;
public:
	Pause(SpriteRenderer* spriteRenderer);

	void Update() override;

	void Draw() override;

	bool IsState(const char* typeName) const;

	bool IsNext() const override;
};

#pragma once
#include <GameLibHeader.h>

using namespace gamelib;
//‰ï˜bƒV[ƒ“
class Talk : public BaseChildScene
{
private:
	float time = 0;
	std::weak_ptr<Texture> texture;
	SpriteRenderer* spriteRenderer;
	FontAnimation fontAnimation;
	std::unique_ptr<SpriteFont> spriteFont;
public:
	Talk(SpriteRenderer* pSpriteRenderer, const std::string& gctext_FileName);
	void Update() override;
	void Draw() override;
	bool IsState(const char* typeName) const;
	bool IsNext() const override;
};


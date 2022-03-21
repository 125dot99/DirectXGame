#include "Talk.h"

#include "InputControl.h"
#include "app/Application.h"

enum TEXTURE
{
	TEX_TALK,
	TEX_TALK_BACK,
};

Talk::Talk(SpriteRenderer* pSpriteRenderer, const std::string& gctext_FileName) : spriteRenderer(pSpriteRenderer)
{
	time = 0;
	texture = ResourceManager::GetInstance()->GetTexture("back.png");
	auto font = ResourceManager::GetInstance()->GetTexture("font_jp_32x32.png");

	spriteFont = Factory::CreateUnique<SpriteFont>(spriteRenderer, font, fontAnimation);
	spriteFont->LoadGctFile(gctext_FileName);
	spriteFont->NextReadText(2);
}

void Talk::Update()
{
	if (input_control::MainAction())
	{
		fontAnimation.animaIndex = fontAnimation.endIndex;
	}
	time += 0.05f;
	time = Max(0.5f, time);
}

void Talk::Draw()
{
	Vector2 center = Application::GetInstance()->GetWindowSize() / 2;
	spriteRenderer->Begin();
	spriteRenderer->DrawExtend(texture, center - Vector2(0, 300), Vector2(720, 220), Vector2(0.5f, 0.0f), Vector4(1, 1, 1, time));
	spriteFont->Draw(Vector2(400, 100), Vector2::One());
}

bool Talk::IsState(const char* typeName) const
{
	return "Talk" == typeName;
}

bool Talk::IsNext() const
{
	return false;
}

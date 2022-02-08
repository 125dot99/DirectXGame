#include "Talk.h"

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

	spriteFont = Factory::CreateUnique<SpriteFont>(spriteRenderer, font);
	spriteFont->LoadGctFile(gctext_FileName);
	spriteFont->NextReadText(2);
}

void Talk::Update()
{
	if (Input::GetKeyborad()->IsKeyDown(KEY_CODE::SPACE) ||
		Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::B))
	{
		spriteFont->NextReadText(2);
	}
	time += 0.1f;
	time = Max(1.0f, time);
}

void Talk::Draw()
{
	Vector2 center = Application::GetInstance()->GetWindowSize() / 2;
	spriteRenderer->Begin();
	spriteRenderer->DrawExtend(texture, center - Vector2(0, 100), Vector2(720, 220), Vector2(0.5f, 0.0f), Vector4(time));
	spriteFont->Draw(Vector2(400, 550), Vector2::One());
}

bool Talk::IsState(const char* typeName) const
{
	return "Talk" == typeName;
}

bool Talk::IsNext() const
{
	return spriteFont->IsEnd();
}

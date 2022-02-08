#include "Pause.h"

#include "input/Input.h"

enum TEXTURE
{
	PAUSE,
};

Pause::Pause(SpriteRenderer* pSpriteRenderer) : spriteRenderer(pSpriteRenderer)
{
	simpleFilter = std::make_unique<SimpleFilter>();
	textures.resize(2);
	textures[PAUSE] = ResourceManager::GetInstance()->GetTexture("pause_ui.png");
	anima.SetIndex(0, 17 * 13);
}

void Pause::Update()
{
	if (Input::GetKeyborad()->IsKeyDown(KEY_CODE::_1) || 
		Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::LEFT))
	{
		simpleFilter->Move(-1);
	} else 
	if (Input::GetKeyborad()->IsKeyDown(KEY_CODE::_2) || 
		Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::RIGHT))
	{
		simpleFilter->Move(+1);
	}
	anima.Update(5);
}

void Pause::Draw()
{
	simpleFilter->Draw();
	spriteRenderer->Begin();
	spriteRenderer->Draw(textures[PAUSE], Vector2(0, 80));
}

bool Pause::IsState(const char* typeName) const
{
	return "Pause" == typeName;
}

bool Pause::IsNext() const
{
	return Input::GetKeyborad()->IsKeyDown(KEY_CODE::X) || 
		Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::X);
}

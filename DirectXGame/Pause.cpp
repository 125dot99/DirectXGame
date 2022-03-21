#include "Pause.h"

#include "input/Input.h"
#include "InputControl.h"
#include "loader/ResourceTextureHelper.h"

enum TEXTURE
{
	PAUSE_CAMERA,
	PAUSE_BACK,
	PAUSE_END,
	MAX,
};

Pause::Pause(SpriteRenderer* pSpriteRenderer) : spriteRenderer(pSpriteRenderer)
{
	selectIndex = 0;
	const auto& resourceManager = ResourceManager::GetInstance();
	textures.resize(MAX);
	textures[PAUSE_CAMERA] = resourceManager->GetTexture("pause_camera.png");
	textures[PAUSE_BACK] = resourceManager->GetTexture("pause_back.png");
	textures[PAUSE_END] = resourceManager->GetTexture("pause_end.png");
	buttonSize = textures[PAUSE_CAMERA].lock()->GetSize();
}

void Pause::Update()
{
	auto& leftThomb = Input::GetGamepad()->GetLeftThumb();
	float y = 0;
	bool prevEnable = stickEnable;
	if (Input::GetKeyborad()->IsKeyDown(KEY_CODE::W) ||
		leftThomb.y > 0)
	{
		y -= 1;
	}
	if (Input::GetKeyborad()->IsKeyDown(KEY_CODE::S) ||
		leftThomb.y < 0)
	{
		y += 1;
	}
	stickEnable = leftThomb.y != 0 ? true : false;
	if (prevEnable && stickEnable)
	{
		y = 0;
	}
	selectIndex = Clamp(static_cast<int>(selectIndex + y), 0, static_cast<int>(PAUSE_END));
	if (input_control::MainAction())
	{
		switch (selectIndex)
		{
		case PAUSE_CAMERA:
			break;
		case PAUSE_BACK:
			nextState = true;
			break;
		case PAUSE_END:
			Application::GetInstance()->ShutDown();
			break;
		default:
			break;
		}
	}
	else if (input_control::CancelAction())
	{
		nextState = true;
	}
}

void Pause::Draw()
{
	spriteRenderer->Begin();
	Vector2 pos = Vector2(160, 90);
	Vector2 offset = Vector2(0, 90);
	for (int i = 0; i < MAX; i++)
	{
		float scale = i == selectIndex ? 1.1f : 0.9f;
		spriteRenderer->DrawExtend(textures[i], pos + offset * i, buttonSize * scale, Vector2(0.5f));
	}
}

bool Pause::IsState(const char* typeName) const
{
	return "Pause" == typeName;
}

bool Pause::IsNext() const
{
	return nextState;
}

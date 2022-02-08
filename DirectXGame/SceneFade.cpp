#include "SceneFade.h"

#include "math/Easing.h"
#include "loader/ResourceManager.h"
#include "app/Application.h"
#include <math/MyMath.h>

SceneFadeIn::SceneFadeIn(SpriteRenderer* pSpriteRenderer) : spriteRenderer(pSpriteRenderer)
{
	time = 0;
	texture = ResourceManager::GetInstance()->GetDefalutTexture();
}

void SceneFadeIn::Update()
{
	const float T = 0.05f;
	time = Max(1.0f, time + T);
}

void SceneFadeIn::Draw()
{
	const auto size = Application::GetInstance()->GetWindowSize();
	spriteRenderer->Begin();
	spriteRenderer->DrawExtend(texture, Vector2::Zero(), size, Vector4(0, 0, 0, time));
}

bool SceneFadeIn::IsState(const char* typeName) const
{
	return "FadeIn" == typeName;
}

bool SceneFadeIn::IsNext() const
{
	return time >= 1.0f;
}

SceneFadeOut::SceneFadeOut(SpriteRenderer* pSpriteRenderer) : spriteRenderer(pSpriteRenderer)
{
	time = 0;
	texture = ResourceManager::GetInstance()->GetDefalutTexture();
}

void SceneFadeOut::Update()
{
	const float T = 0.1f;
	time = Max(1.0f, time + T);
}

void SceneFadeOut::Draw()
{
	const auto size = Application::GetInstance()->GetWindowSize();
	spriteRenderer->Begin();
	spriteRenderer->DrawExtend(texture, Vector2::Zero(), size, Vector4(0, 0, 0, 1.0f - time));
}

bool SceneFadeOut::IsState(const char* typeName) const
{
	return "FadeOut" == typeName;
}

bool SceneFadeOut::IsNext() const
{
	return time >= 1.0f;
}

SceneNextCafeWait::SceneNextCafeWait(SpriteRenderer* pSpriteRenderer) : spriteRenderer(pSpriteRenderer)
{
	time = 0;
	texture = ResourceManager::GetInstance()->GetDefalutTexture();
	cafeTexture = ResourceManager::GetInstance()->GetTexture("char_talk.png");
}

void SceneNextCafeWait::Update()
{
	const float T = 0.1f;
	time = Max(1.0f, time + T);
}

void SceneNextCafeWait::Draw()
{
	const auto size = Application::GetInstance()->GetWindowSize();
	spriteRenderer->Begin();
	spriteRenderer->DrawExtend(texture, Vector2::Zero(), size, Vector4(0, 0, 0, 1));

}

bool SceneNextCafeWait::IsState(const char* typeName) const
{
	return "NextWait" == typeName;
}

bool SceneNextCafeWait::IsNext() const
{
	return false;
}

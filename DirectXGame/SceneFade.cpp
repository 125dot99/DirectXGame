#include "SceneFade.h"

#include "math/Easing.h"
#include "loader/ResourceManager.h"
#include "app/Application.h"
#include <math/MyMath.h>

SceneFadeIn::SceneFadeIn(SpriteRenderer* pSpriteRenderer) : spriteRenderer(pSpriteRenderer)
{
	time = 0;
	w_pWhiteTexture = ResourceManager::GetInstance()->GetDefalutTexture();
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
	spriteRenderer->DrawExtend(w_pWhiteTexture, Vector2::Zero(), size, Vector4(0, 0, 0, time));
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
	w_pWhiteTexture = ResourceManager::GetInstance()->GetDefalutTexture();
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
	spriteRenderer->DrawExtend(w_pWhiteTexture, Vector2::Zero(), size, Vector4(0, 0, 0, 1.0f - time));
}

bool SceneFadeOut::IsState(const char* typeName) const
{
	return "FadeOut" == typeName;
}

bool SceneFadeOut::IsNext() const
{
	return time >= 1.0f;
}

SceneNextCafeWait::SceneNextCafeWait(SpriteRenderer* pSpriteRenderer, bool isNight) : spriteRenderer(pSpriteRenderer)
{
	timer.Begin();
	rotation = offset = isNight ? 0 : math::PI;
	w_pWhiteTexture = ResourceManager::GetInstance()->GetDefalutTexture();
	cafeTexture = ResourceManager::GetInstance()->GetTexture("noon_and_night.png");
}

void SceneNextCafeWait::Update()
{
	float frame = timer.ElapsedSec();
	//if (frame >= 1.62f)
	//{
	//	frame = 0;
	//	offset = offset >= math::PI ? 0 : math::PI;
	//	timer.Begin();
	//}
	rotation = Lerp(0.0f, math::PI, easing::EaseInOutExpo(frame)) + offset;
	timer.End();
}

void SceneNextCafeWait::Draw()
{
	const auto size = Application::GetInstance()->GetWindowSize();
	spriteRenderer->Begin();
	spriteRenderer->DrawExtend(w_pWhiteTexture, Vector2::Zero(), size, Vector4(0, 0, 0, 1));
	spriteRenderer->DrawRotate(cafeTexture, size / 2, rotation, Vector2(0.5f, 0.5f));
}

bool SceneNextCafeWait::IsState(const char* typeName) const
{
	return "NextWait" == typeName;
}

bool SceneNextCafeWait::IsNext() const
{
	return timer.ElapsedSec() >= 1.5f;
}

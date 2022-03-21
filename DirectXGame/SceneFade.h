#pragma once
#include "scene/BaseScene.h"
#include "renderer/SpriteRenderer.h"
#include "app/Timer.h"

using namespace gamelib;

class SceneFadeIn : public BaseChildScene
{
private:
	SpriteRenderer* spriteRenderer;
	std::weak_ptr<Texture> w_pWhiteTexture;
	float time;
public:
	SceneFadeIn(SpriteRenderer* pSpriteRenderer);
	virtual void Update() override;
	virtual void Draw() override;
	virtual bool IsState(const char* typeName) const override;
	virtual bool IsNext() const override;
};

class SceneFadeOut : public BaseChildScene
{
private:
	SpriteRenderer* spriteRenderer;
	std::weak_ptr<Texture> w_pWhiteTexture;
	float time;
public:
	SceneFadeOut(SpriteRenderer* pSpriteRenderer);
	virtual void Update() override;
	virtual void Draw() override;
	virtual bool IsState(const char* typeName) const override;
	virtual bool IsNext() const override;
};

class SceneNextCafeWait : public BaseChildScene
{
private:
	float offset;
	float rotation;
	Timer timer;
	SpriteRenderer* spriteRenderer;
	std::weak_ptr<Texture> w_pWhiteTexture;
	std::weak_ptr<Texture> cafeTexture;

	Vector2 startPos;
	Vector2 endPos;
public:
	SceneNextCafeWait(SpriteRenderer* pSpriteRenderer, bool isNight);
	virtual void Update() override;
	virtual void Draw() override;
	virtual bool IsState(const char* typeName) const override;
	virtual bool IsNext() const override;
};
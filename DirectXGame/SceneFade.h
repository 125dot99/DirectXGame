#pragma once
#include "scene/BaseScene.h"
#include "renderer/SpriteRenderer.h"
using namespace gamelib;

class SceneFadeIn : public BaseChildScene
{
private:
	SpriteRenderer* spriteRenderer;
	std::weak_ptr<Texture> texture;
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
	std::weak_ptr<Texture> texture;
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
	SpriteRenderer* spriteRenderer;
	std::weak_ptr<Texture> texture;
	std::weak_ptr<Texture> cafeTexture;

	Vector2 startPos;
	Vector2 endPos;

	float time;
public:
	SceneNextCafeWait(SpriteRenderer* pSpriteRenderer);
	virtual void Update() override;
	virtual void Draw() override;
	virtual bool IsState(const char* typeName) const override;
	virtual bool IsNext() const override;
};
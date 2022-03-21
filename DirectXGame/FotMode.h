#pragma once
#include <GameLibHeader.h>
#include "SimpleFilter.h"
#include "dx12/GpuCopyPixels.h"

using namespace gamelib;

class FotMode : public BaseChildScene
{
private:
	bool isOperation;
	bool isScreenShot;
	SpriteAnimation anima;
	SpriteRenderer* spriteRenderer;

	std::unique_ptr<SimpleFilter> simpleFilter;
	std::unique_ptr<GpuCopyPixels> gpuCopyPixels;
	std::vector<std::weak_ptr<Texture>> textures;

public:
    FotMode(SpriteRenderer* pSpriteRenderer);
	void Update() override;
	void Draw() override;
	bool IsState(const char* typeName) const;
	bool IsNext() const override;
};


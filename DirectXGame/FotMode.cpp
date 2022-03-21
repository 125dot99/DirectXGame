#include "FotMode.h"

#include "input/Input.h"
#include "InputControl.h"
#include "loader/ResourceTextureHelper.h"

enum TEXTURE
{
	PAUSE,
	FILTER_NONE,
	FILTER_GRAY_COLOR,
	FILTER_SEPIA_COLOR,
	FILTER_MOSAIC,
	FILTER_GRAIN,
	FILTER_GAUSS_BLUR,
	FILTER_BLOOM,
};

FotMode::FotMode(SpriteRenderer* pSpriteRenderer) : spriteRenderer(pSpriteRenderer)
{
	isOperation = true;
	isScreenShot = false;

	const auto& resourceManager = ResourceManager::GetInstance();
	simpleFilter = std::make_unique<SimpleFilter>();
	gpuCopyPixels = std::make_unique<GpuCopyPixels>();
	textures.resize(FILTER_BLOOM + 1);
	textures[PAUSE] = resourceManager->GetTexture("fotmode.png");
	textures[FILTER_NONE] = resourceManager->GetTexture("filter_none.png");
	textures[FILTER_GRAY_COLOR] = resourceManager->GetTexture("filter_gray.png");
	textures[FILTER_SEPIA_COLOR] = resourceManager->GetTexture("filter_sepia.png");
	textures[FILTER_MOSAIC] = resourceManager->GetTexture("filter_mosaic.png");
	textures[FILTER_GRAIN] = resourceManager->GetTexture("filter_grain.png");
	textures[FILTER_GAUSS_BLUR] = resourceManager->GetTexture("filter_blur.png");
	textures[FILTER_BLOOM] = resourceManager->GetTexture("filter_bloom.png");
	anima.SetIndex(0, 17 * 13);
}

void FotMode::Update()
{
	//if (input_control::MainAction())
	//{
	//	isScreenShot = true;
	//}
	if (input_control::SubAction())
	{
		isOperation = !isOperation;
	}
	if (input_control::PauseFilterLeft())
	{
		simpleFilter->Move(-1);
	}
	if (input_control::PauseFilterRight())
	{
		simpleFilter->Move(+1);
	}
	anima.Update(5);
}

void FotMode::Draw()
{
	simpleFilter->Draw();
	//現在のシーンを画像データにして保存
	//if (isScreenShot)
	//{
	//	isScreenShot = false;
	//	const auto& image = gpuCopyPixels->Dispatch(RenderManager::GetInstance()->GetRenderTarget(0));
	//	ResourceTextureHelper::SaveTextureBuffer("test1.png", image);
	//}
	if (isOperation)
	{
		spriteRenderer->Begin();
		Vector2 size = Application::GetInstance()->GetWindowSize();
		spriteRenderer->Draw(textures[PAUSE], Vector2::Zero());
		int filterIndex = simpleFilter->GetFilterIndex() + 1;
		spriteRenderer->Draw(textures[filterIndex], Vector2(size.x - 40, 40), Vector2(1.0f, 0));
	}
}

bool FotMode::IsState(const char* typeName) const
{
	return "FotMode" == typeName;
}

bool FotMode::IsNext() const
{
	return input_control::CancelAction();
}

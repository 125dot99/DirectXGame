#include "PostEffectData.h"

#include "../pipeline/PipelineManager.h"
#include "../posteffect/RenderManager.h"
#include "../app/Application.h"

namespace gamelib
{
std::unique_ptr<RenderTarget> InitTemplate()
{
	auto newTexture = std::make_unique<RenderTarget>(Vector2::Zero());
	RenderManager::GetInstance()->Create(newTexture.get(), 10);
	return newTexture;
}

void ProsscessTemplate(const std::string& pipelineName, RenderTarget* pRenderTarget)
{
	pRenderTarget->RBRenderTarget();
	PipelineManager::GetInstance()->GetPipelineState(pipelineName).lock()->Command();
	RenderManager::GetInstance()->GetRenderTarget(0).lock()->GraphicsSRVCommand(0);
	RenderManager::GetInstance()->Draw();
	pRenderTarget->RBPixelShaderResource();

	RenderManager::GetInstance()->WriteStart();
	PipelineManager::GetInstance()->GetPipelineState("PE_None").lock()->Command();
	pRenderTarget->GraphicsSRVCommand(0);
	RenderManager::GetInstance()->Draw();
}
} // namespace gamelib

gamelib::GrayScale::GrayScale()
{
	u_pRenderTarget = InitTemplate();
}

void gamelib::GrayScale::Befoer()
{
	RenderManager::GetInstance()->WriteEnd();
}

void gamelib::GrayScale::After()
{
	ProsscessTemplate("PE_Gray", u_pRenderTarget.get());
}

gamelib::Sepia::Sepia()
{
	u_pRenderTarget = InitTemplate();
}

void gamelib::Sepia::Befoer()
{
	RenderManager::GetInstance()->WriteEnd();
}

void gamelib::Sepia::After()
{
	ProsscessTemplate("PE_Sepia", u_pRenderTarget.get());
}

void gamelib::Vignetting::Befoer()
{

}

void gamelib::Vignetting::After()
{
	PipelineManager::GetInstance()->GetPipelineState("PE_Vignet").lock()->Command();
	RenderManager::GetInstance()->Draw();
}

gamelib::Mosaic::Mosaic()
{
	u_pRenderTarget = InitTemplate();
}
void gamelib::Mosaic::Befoer()
{
	RenderManager::GetInstance()->WriteEnd();
}

void gamelib::Mosaic::After()
{
	ProsscessTemplate("PE_Mosaic", u_pRenderTarget.get());
}

gamelib::Grain::Grain()
{
	u_pRenderTarget = InitTemplate();
}

void gamelib::Grain::Befoer()
{
	RenderManager::GetInstance()->WriteEnd();
}

void gamelib::Grain::After()
{
	ProsscessTemplate("PE_Grain", u_pRenderTarget.get());
}

gamelib::RetroFilter::RetroFilter()
{
	u_pRenderTarget = InitTemplate();
}

void gamelib::RetroFilter::Befoer()
{
	RenderManager::GetInstance()->WriteEnd();
}

void gamelib::RetroFilter::After()
{
	ProsscessTemplate("PE_Retro", u_pRenderTarget.get());
}

gamelib::GaussFilter::GaussFilter()
{
	u_pGaussianBlur = std::make_unique<GaussianBlur>();
	u_pGaussianBlur->Init(5.0f);
	u_pGaussianBlur->Map();
	Vector2 screen = Application::GetInstance()->GetWindowSize();
	u_pRenderTargets[0] = std::make_unique<RenderTarget>(Vector2(screen.x / 2, screen.y));
	u_pRenderTargets[1] = std::make_unique<RenderTarget>(Vector2(screen.x / 2, screen.y / 2));
	RenderManager::GetInstance()->Create(u_pRenderTargets[0].get(), 10);
	RenderManager::GetInstance()->Create(u_pRenderTargets[1].get(), 11);

	auto pipelineManager = PipelineManager::GetInstance();
	w_pRenderPipeline = pipelineManager->GetPipelineState("PE_None");
	w_pBlurWPipeline = pipelineManager->GetPipelineState("PE_BlurW");
	w_pBlurHPipeline = pipelineManager->GetPipelineState("PE_BlurH");
}

void gamelib::GaussFilter::Befoer()
{
	RenderManager::GetInstance()->WriteEnd();
}

void gamelib::GaussFilter::After()
{
	u_pRenderTargets[0]->RBRenderTarget();
	w_pBlurWPipeline.lock()->Command();
	u_pGaussianBlur->Command();
	RenderManager::GetInstance()->GetRenderTarget(0).lock()->GraphicsSRVCommand(1);
	RenderManager::GetInstance()->Draw();
	u_pRenderTargets[0]->RBPixelShaderResource();

	u_pRenderTargets[1]->RBRenderTarget();
	w_pBlurHPipeline.lock()->Command();
	u_pGaussianBlur->Command();
	u_pRenderTargets[0]->GraphicsSRVCommand(1);
	RenderManager::GetInstance()->Draw();
	u_pRenderTargets[1]->RBPixelShaderResource();

	RenderManager::GetInstance()->WriteStart();
	w_pRenderPipeline.lock()->Command();
	u_pRenderTargets[1]->GraphicsSRVCommand(0);
	RenderManager::GetInstance()->Draw();
}

gamelib::Bloom::Bloom()
{
	u_pGaussianBlur = std::make_unique<GaussianBlur>();
	u_pGaussianBlur->Init(40.0f);
	u_pGaussianBlur->Map();

	Vector2 screen = Application::GetInstance()->GetWindowSize();
	u_pHighlimRenderTarget = std::make_unique<RenderTarget>(screen, DXGI_FORMAT_R8_UNORM);
	RenderManager::GetInstance()->Create(u_pHighlimRenderTarget.get(), 10);
	const int Index = 10 + 1;
	u_pRenderTargets[0] = std::make_unique<RenderTarget>(Vector2(screen.x / 2, screen.y), DXGI_FORMAT_R32_FLOAT);
	RenderManager::GetInstance()->Create(u_pRenderTargets[0].get(), Index);
	u_pRenderTargets[1] = std::make_unique<RenderTarget>(Vector2(screen.x / 2, screen.y / 2), DXGI_FORMAT_R32_FLOAT);
	RenderManager::GetInstance()->Create(u_pRenderTargets[1].get(), Index + 1);

	auto pipelineManager = PipelineManager::GetInstance();
	w_pHighlimPipeline = pipelineManager->GetPipelineState("PE_HighLim");
	w_pBlurWPipeline = pipelineManager->GetPipelineState("PE_R32BlurW");
	w_pBlurHPipeline = pipelineManager->GetPipelineState("PE_R32BlurH");
	w_pBloomPipeline = pipelineManager->GetPipelineState("PE_Bloom");
}

void gamelib::Bloom::Befoer()
{
	RenderManager::GetInstance()->WriteEnd();
}

void gamelib::Bloom::After()
{
	u_pHighlimRenderTarget->RBRenderTarget(true);
	w_pHighlimPipeline.lock()->Command();
	RenderManager::GetInstance()->GetRenderTarget(0).lock()->GraphicsSRVCommand(0);
	RenderManager::GetInstance()->Draw();
	u_pHighlimRenderTarget->RBPixelShaderResource();
	
	u_pRenderTargets[0]->RBRenderTarget();
	w_pBlurWPipeline.lock()->Command();
	u_pGaussianBlur->Command();
	u_pHighlimRenderTarget->GraphicsSRVCommand(1);
	RenderManager::GetInstance()->Draw();
	u_pRenderTargets[0]->RBPixelShaderResource();

	u_pRenderTargets[1]->RBRenderTarget();
	w_pBlurHPipeline.lock()->Command();
	u_pGaussianBlur->Command();
	u_pRenderTargets[0]->GraphicsSRVCommand(1);
	RenderManager::GetInstance()->Draw();
	u_pRenderTargets[1]->RBPixelShaderResource();

	RenderManager::GetInstance()->WriteStart(false, true);
	w_pBloomPipeline.lock()->Command();
	u_pRenderTargets[1]->GraphicsSRVCommand(0);
	RenderManager::GetInstance()->Draw();
}

gamelib::DepthOfField::DepthOfField()
{

}

void gamelib::DepthOfField::Befoer()
{

}

void gamelib::DepthOfField::After()
{

}

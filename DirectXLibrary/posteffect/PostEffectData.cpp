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

void ProsscessTemplate(const std::string& _pipelineTag, RenderTarget* pTexture)
{
	pTexture->RBRenderTarget();
	PipelineManager::GetInstance()->GetPipelineState(_pipelineTag)->Command();
	RenderManager::GetInstance()->GetRenderTarget(0)->GraphicsSRVCommand(0);
	RenderManager::GetInstance()->Draw();
	pTexture->RBPixelShaderResource();

	RenderManager::GetInstance()->WriteStart();
	PipelineManager::GetInstance()->GetPipelineState("PE_None")->Command();
	pTexture->GraphicsSRVCommand(0);
	RenderManager::GetInstance()->Draw();
}
} // namespace gamelib

gamelib::GrayScale::GrayScale()
{
	render = InitTemplate();
}

void gamelib::GrayScale::Befoer()
{
	RenderManager::GetInstance()->WriteEnd();
}

void gamelib::GrayScale::After()
{
	ProsscessTemplate("PE_Gray", render.get());
}

gamelib::Sepia::Sepia()
{
	render = InitTemplate();
}

void gamelib::Sepia::Befoer()
{
	RenderManager::GetInstance()->WriteEnd();
}

void gamelib::Sepia::After()
{
	ProsscessTemplate("PE_Sepia", render.get());
}

void gamelib::Vignetting::Befoer()
{

}

void gamelib::Vignetting::After()
{
	PipelineManager::GetInstance()->GetPipelineState("PE_Vignet")->Command();
	RenderManager::GetInstance()->Draw();
}

gamelib::Mosaic::Mosaic()
{
	render = InitTemplate();
}
void gamelib::Mosaic::Befoer()
{
	RenderManager::GetInstance()->WriteEnd();
}

void gamelib::Mosaic::After()
{
	ProsscessTemplate("PE_Mosaic", render.get());
}

gamelib::Grain::Grain()
{
	render = InitTemplate();
}

void gamelib::Grain::Befoer()
{
	RenderManager::GetInstance()->WriteEnd();
}

void gamelib::Grain::After()
{
	ProsscessTemplate("PE_Grain", render.get());
}

gamelib::RetroFilter::RetroFilter()
{
	render = InitTemplate();
}

void gamelib::RetroFilter::Befoer()
{
	RenderManager::GetInstance()->WriteEnd();
}

void gamelib::RetroFilter::After()
{
	ProsscessTemplate("PE_Retro", render.get());
}

gamelib::GaussFilter::GaussFilter()
{
	gauss = std::make_unique<GaussianBlur>();
	gauss->Init(5.0f);
	gauss->Map();
	Vector2 screen = Application::GetInstance()->GetWindowSize();
	textures[0] = std::make_unique<RenderTarget>(Vector2(screen.x / 2, screen.y));
	textures[1] = std::make_unique<RenderTarget>(Vector2(screen.x / 2, screen.y / 2));
	RenderManager::GetInstance()->Create(textures[0].get(), 10);
	RenderManager::GetInstance()->Create(textures[1].get(), 11);

	auto pipelineManager = PipelineManager::GetInstance();
	renderPipeline = pipelineManager->GetPipelineState("PE_None");
	blurWPipeline = pipelineManager->GetPipelineState("PE_BlurW");
	blurHPipeline = pipelineManager->GetPipelineState("PE_BlurH");
}

void gamelib::GaussFilter::Befoer()
{
	RenderManager::GetInstance()->WriteEnd();
}

void gamelib::GaussFilter::After()
{
	textures[0]->RBRenderTarget();
	blurWPipeline->Command();
	gauss->Command();
	RenderManager::GetInstance()->GetRenderTarget(0)->GraphicsSRVCommand(1);
	RenderManager::GetInstance()->Draw();
	textures[0]->RBPixelShaderResource();

	textures[1]->RBRenderTarget();
	blurHPipeline->Command();
	gauss->Command();
	textures[0]->GraphicsSRVCommand(1);
	RenderManager::GetInstance()->Draw();
	textures[1]->RBPixelShaderResource();

	RenderManager::GetInstance()->WriteStart();
	renderPipeline->Command();
	textures[1]->GraphicsSRVCommand(0);
	RenderManager::GetInstance()->Draw();
}

gamelib::Bloom::Bloom()
{
	gauss = std::make_unique<GaussianBlur>();
	gauss->Init(40.0f);
	gauss->Map();

	Vector2 screen = Application::GetInstance()->GetWindowSize();
	highTex = std::make_unique<RenderTarget>(screen, DXGI_FORMAT_R8_UNORM);
	RenderManager::GetInstance()->Create(highTex.get(), 10);
	const int Index = 10 + 1;
	textures[0] = std::make_unique<RenderTarget>(Vector2(screen.x / 2, screen.y), DXGI_FORMAT_R32_FLOAT);
	RenderManager::GetInstance()->Create(textures[0].get(), Index);
	textures[1] = std::make_unique<RenderTarget>(Vector2(screen.x / 2, screen.y / 2), DXGI_FORMAT_R32_FLOAT);
	RenderManager::GetInstance()->Create(textures[1].get(), Index + 1);

	auto pipelineManager = PipelineManager::GetInstance();
	highlimPipeline = pipelineManager->GetPipelineState("PE_HighLim");
	hdBlurWPipeline = pipelineManager->GetPipelineState("PE_R32BlurW");
	hdBlurHPipeline = pipelineManager->GetPipelineState("PE_R32BlurH");
	bloomPipeline = pipelineManager->GetPipelineState("PE_Bloom");
}

void gamelib::Bloom::Befoer()
{
	RenderManager::GetInstance()->WriteEnd();
}

void gamelib::Bloom::After()
{
	highTex->RBRenderTarget(true);
	highlimPipeline->Command();
	RenderManager::GetInstance()->GetRenderTarget(0)->GraphicsSRVCommand(0);
	RenderManager::GetInstance()->Draw();
	highTex->RBPixelShaderResource();
	
	textures[0]->RBRenderTarget();
	hdBlurWPipeline->Command();
	gauss->Command();
	highTex->GraphicsSRVCommand(1);
	RenderManager::GetInstance()->Draw();
	textures[0]->RBPixelShaderResource();

	textures[1]->RBRenderTarget();
	hdBlurHPipeline->Command();
	gauss->Command();
	textures[0]->GraphicsSRVCommand(1);
	RenderManager::GetInstance()->Draw();
	textures[1]->RBPixelShaderResource();

	RenderManager::GetInstance()->WriteStart(false, true);
	bloomPipeline->Command();
	textures[1]->GraphicsSRVCommand(0);
	RenderManager::GetInstance()->Draw();
}

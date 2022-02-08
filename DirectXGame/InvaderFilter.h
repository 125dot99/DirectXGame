#pragma once
#include "posteffect/IPostProcess.h"
#include "posteffect/GaussianBlur.h"
#include "pipeline/IPipelineState.h"
#include "dx12/RenderTarget.h"

using namespace gamelib;

class InvaderFilter : public IPostProcess
{
private:
	IPipelineState* invaderPipeline;
	IPipelineState* highlimPipeline;
	IPipelineState* hdBlurWPipeline;
	IPipelineState* hdBlurHPipeline;
	IPipelineState* bloomPipeline;
	
	std::weak_ptr<Texture> colorTexture;
	std::unique_ptr<RenderTarget> render;

	std::unique_ptr<GaussianBlur> gauss;
	std::unique_ptr<RenderTarget> highTex;
	std::unique_ptr<RenderTarget> textures[2];
public:
	InvaderFilter(const std::weak_ptr<Texture>& w_p_Texture);
	void Befoer() override;
	void After() override;
};

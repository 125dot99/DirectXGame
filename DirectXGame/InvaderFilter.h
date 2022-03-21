#pragma once
#include "posteffect/IPostProcess.h"
#include "pipeline/IPipelineState.h"
#include "dx12/RenderTarget.h"

using namespace gamelib;

class InvaderFilter : public IPostProcess
{
private:
	std::unique_ptr<RenderTarget> u_pRenderTarget;
	std::weak_ptr<Texture> w_pColorTexture;
	std::weak_ptr<IPipelineState> w_pPipeline;

public:
	InvaderFilter(const std::weak_ptr<Texture>& w_pTexture);
	void Befoer() override;
	void After() override;
};

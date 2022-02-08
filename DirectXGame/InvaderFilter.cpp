#include "InvaderFilter.h"

#include "pipeline/PipelineManager.h"
#include "posteffect/RenderManager.h"

InvaderFilter::InvaderFilter(const std::weak_ptr<Texture>& w_p_Texture)
{
	colorTexture = w_p_Texture;
	render = std::make_unique<RenderTarget>(Vector2::Zero());
	RenderManager::GetInstance()->Create(render.get(), 10);
}

void InvaderFilter::Befoer()
{
}

void InvaderFilter::After()
{
	RenderManager::GetInstance()->WriteEnd();

	render->RBRenderTarget();
	PipelineManager::GetInstance()->GetPipelineState("PE_Inverder")->Command();
	RenderManager::GetInstance()->GetRenderTarget(0)->GraphicsSRVCommand(0);
	colorTexture.lock()->GraphicsSRVCommand(1);
	RenderManager::GetInstance()->Draw();
	render->RBPixelShaderResource();

	RenderManager::GetInstance()->WriteStart();
	PipelineManager::GetInstance()->GetPipelineState("PE_None")->Command();
	render->GraphicsSRVCommand(0);
	RenderManager::GetInstance()->Draw();
}

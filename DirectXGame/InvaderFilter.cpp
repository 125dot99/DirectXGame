#include "InvaderFilter.h"

#include "pipeline/PipelineManager.h"
#include "posteffect/RenderManager.h"

InvaderFilter::InvaderFilter(const std::weak_ptr<Texture>& w_pTexture)
{
	w_pColorTexture = w_pTexture;
	u_pRenderTarget = std::make_unique<RenderTarget>(Vector2::Zero());
	RenderManager::GetInstance()->Create(u_pRenderTarget.get(), 10);
	w_pPipeline = PipelineManager::GetInstance()->GetPipelineState("PE_Inverder");
}

void InvaderFilter::Befoer()
{

}

void InvaderFilter::After()
{
	const auto& rendering = RenderManager::GetInstance();
	rendering->WriteEnd();

	u_pRenderTarget->RBRenderTarget();
	w_pPipeline.lock()->Command();
	rendering->GetRenderTarget(0).lock()->GraphicsSRVCommand(0);
	w_pColorTexture.lock()->GraphicsSRVCommand(1);
	rendering->Draw();
	u_pRenderTarget->RBPixelShaderResource();

	rendering->WriteStart();
	PipelineManager::GetInstance()->GetPipelineState("PE_None").lock()->Command();
	u_pRenderTarget->GraphicsSRVCommand(0);
	rendering->Draw();
}

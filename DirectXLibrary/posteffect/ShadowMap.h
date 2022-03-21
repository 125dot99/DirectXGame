#pragma once
#include "../dx12/RenderTarget.h"
#include "../posteffect/IPostProcess.h"
#include "../pipeline/IPipelineState.h"
#include "../dx12/ConstBuffer.h"
#include "../math/Matrix4.h"

namespace gamelib
{
class ShadowMap : public IPostProcess
{
private:
	Matrix4 matShadow;
	std::unique_ptr<ConstBuffer> u_pConstBuffer;
	std::shared_ptr<RenderTarget> s_pRenderTarget;
	std::weak_ptr<IPipelineState> w_pPipeline;

public:
	ShadowMap(const Vector3& lightV, const float groundY);
	void Befoer() override;
	void After() override;
};
} // namespace gamelib

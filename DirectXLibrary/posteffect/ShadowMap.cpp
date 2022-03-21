#include "ShadowMap.h"

#include "RenderManager.h"
#include "../app/Application.h"

gamelib::ShadowMap::ShadowMap(const Vector3& lightV, const float groundY)
{
	matShadow = MatrixShadow(lightV, groundY);
	u_pConstBuffer = std::make_unique<ConstBuffer>();
	u_pConstBuffer->Init((UINT)ROOT_PARAMETER::MATERIAL, sizeof(Matrix4));
	u_pConstBuffer->Map(&matShadow);

	s_pRenderTarget = std::make_shared<RenderTarget>(Vector2(512), DXGI_FORMAT_R8G8B8A8_UNORM, Vector4(1, 1, 1, 1));
	RenderManager::GetInstance()->Add(s_pRenderTarget);
}

void gamelib::ShadowMap::Befoer()
{
	const auto& renderManager = RenderManager::GetInstance();
	renderManager->WriteEnd();
	s_pRenderTarget->RBRenderTarget(true, true);
}

void gamelib::ShadowMap::After()
{
	const auto& renderManager = RenderManager::GetInstance();
	u_pConstBuffer->GraphicsCommand();
	s_pRenderTarget->RBPixelShaderResource();
	renderManager->WriteStart(true, true);
}

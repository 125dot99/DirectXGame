#include "WaveMaterial.h"

#include "WaveSimulate.h"
#include "pipeline/IPipelineState.h"
#include "posteffect/RenderManager.h"

struct CBTessellationParam
{
	Matrix4 refMat;
	Vector3 factor;
	CBTessellationParam(const Matrix4& refMat, const Vector3& factor) :
		refMat(refMat), factor(factor) {}
};

Matrix4 CalcReflectionMatrix(const Vector4& normal)
{
	Matrix4 refMat;
	refMat.m[0][0] = 1.0f - 2.0f * normal.x * normal.x;
	refMat.m[0][1] = -2.0f * normal.x * normal.y;
	refMat.m[0][2] = -2.0f * normal.x * normal.z;
	refMat.m[0][3] = -2.0f * normal.x * normal.w;
	refMat.m[1][0] = -2.0f * normal.y * normal.x;
	refMat.m[1][1] = 1.0f - 2.0f * normal.y * normal.y;
	refMat.m[1][2] = -2.0f * normal.y * normal.z;
	refMat.m[1][3] = -2.0f * normal.y * normal.w;
	refMat.m[2][0] = -2.0f * normal.z * normal.x;
	refMat.m[2][1] = -2.0f * normal.z * normal.y;
	refMat.m[2][2] = 1.0f - 2.0f * normal.z * normal.z;
	refMat.m[2][3] = -2.0f * normal.z * normal.w;
	refMat.m[3][0] = refMat.m[3][1] = refMat.m[3][2] = 0.0f;
	refMat.m[3][3] = 1.0f;
	return refMat;
}

void WaveMaterial::Create()
{
	refMatrix = CalcReflectionMatrix(Vector4(0, 1, 0, 0));
	tessFactor = Vector3(10.0f, 30.0f, 0);
	
	u_pConstBuffer = std::make_unique<ConstBuffer>();
	u_pConstBuffer->Init((UINT)ROOT_PARAMETER::MATERIAL, sizeof(CBTessellationParam));
	CBTessellationParam buff(refMatrix, tessFactor);
	u_pConstBuffer->Map(&buff);

	w_pHighTexture = RenderManager::GetInstance()->GetRenderTarget(WAVE_TEX + 1);
	w_pBumpTexture = RenderManager::GetInstance()->GetRenderTarget(NORMAL_TEX + 1);
	w_pRefTexture = RenderManager::GetInstance()->GetRenderTarget(REFLECT_TEX + 1);
}

void WaveMaterial::Register()
{
	u_pConstBuffer->GraphicsCommand();
	UINT index = (UINT)ROOT_PARAMETER::MATERIAL;
	w_pHighTexture.lock()->GraphicsSRVCommand(index + 1);
	w_pBumpTexture.lock()->GraphicsSRVCommand(index + 2);
	w_pRefTexture.lock()->GraphicsSRVCommand(index + 3);
}

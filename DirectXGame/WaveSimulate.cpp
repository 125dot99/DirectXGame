#include "WaveSimulate.h"

#include "math/MyMath.h"
#include "posteffect/RenderManager.h"
#include "pipeline/PipelineManager.h"

struct PaintCbData
{
	Vector4 pos;
	Vector2 screen;
	PaintCbData(const Vector3& pos, const Vector2& screen) :
		pos(pos), screen(screen) {}
};

WaveSimulates::WaveSimulates(GameObject* pPaintObject, const Vector2& _textureSize, float meshSize) : pPaintObject(pPaintObject)
{
	waveMeshSize = meshSize;
	//バッファの生成
	u_pConstBuffer = std::make_unique<ConstBuffer>();
	u_pConstBuffer->Init(0, sizeof(PaintCbData));
	for (int i = 0; i < REFLECT_TEX; i++)
	{
		if (i != NORMAL_TEX)
		{
			s_pRenderTargets[i] = std::make_shared<RenderTarget>(_textureSize, DXGI_FORMAT_R32_FLOAT);
			RenderManager::GetInstance()->Add(s_pRenderTargets[i]);
			continue;
		}
		s_pRenderTargets[i] = std::make_shared<RenderTarget>(_textureSize);
		RenderManager::GetInstance()->Add(s_pRenderTargets[i]);
	}
	for (int i = 0; i < 3; i++)
	{
		s_pRenderTargets[i + 1]->RBRenderTarget();
		PipelineManager::GetInstance()->GetPipelineState("WaveInitShader").lock()->Command();
		RenderManager::GetInstance()->Draw();
		s_pRenderTargets[i + 1]->RBPixelShaderResource();
	}
}

void WaveSimulates::Befoer()
{

}

void WaveSimulates::After()
{
	const auto& renderManager = RenderManager::GetInstance();
	renderManager->WriteEnd();
	const auto& pPipeline = PipelineManager::GetInstance();
	{
		const float s = 0.35f;
		Vector3 pos = pPaintObject->GetPosition();
		Vector2 rot = Vector2::FromAngle(pPaintObject->GetRotation().y);
		pos += Vector3(rot.y, 0, rot.x).Normalize() * s;
		pos.x *= 1.0f / waveMeshSize.x;
		pos.z *= 1.0f / waveMeshSize.y;
		pos += Vector3(0.5f, 0, 0.5f);
		pos.z = 1.0f - pos.z;
		PaintCbData p(pos, s_pRenderTargets[(int)WAVE_TEXTURE_ENUM::INPUT_TEX]->GetSize());
		u_pConstBuffer->Map(&p);
	}
	//0入力バッファ
	s_pRenderTargets[INPUT_TEX]->RBRenderTarget(true, false);
	pPipeline->GetPipelineState("WavePaintShader").lock()->Command();
	u_pConstBuffer->GraphicsCommand();
	renderManager->Draw();
	s_pRenderTargets[INPUT_TEX]->RBPixelShaderResource();

	s_pRenderTargets[WAVE_PREV_PREV_TEX]->RBRenderTarget();
	pPipeline->GetPipelineState("PE_R32None").lock()->Command();
	s_pRenderTargets[WAVE_PREV_TEX]->GraphicsSRVCommand(0);
	renderManager->Draw();
	s_pRenderTargets[WAVE_PREV_PREV_TEX]->RBPixelShaderResource();

	s_pRenderTargets[WAVE_PREV_TEX]->RBRenderTarget();
	pPipeline->GetPipelineState("PE_R32None").lock()->Command();
	s_pRenderTargets[WAVE_TEX]->GraphicsSRVCommand(0);
	renderManager->Draw();
	s_pRenderTargets[WAVE_PREV_TEX]->RBPixelShaderResource();

	//合成
	s_pRenderTargets[WAVE_TEX]->RBRenderTarget();
	pPipeline->GetPipelineState("WaveSimulateShader").lock()->Command();
	for (int i = 0; i < WAVE_TEX; i++)
	{
		s_pRenderTargets[i]->GraphicsSRVCommand(i);
	}
	renderManager->Draw();
	s_pRenderTargets[WAVE_TEX]->RBPixelShaderResource();

	//normal
	s_pRenderTargets[NORMAL_TEX]->RBRenderTarget();
	pPipeline->GetPipelineState("HighNormal").lock()->Command();
	s_pRenderTargets[WAVE_TEX]->GraphicsSRVCommand(0);
	renderManager->Draw();
	s_pRenderTargets[NORMAL_TEX]->RBPixelShaderResource();
	renderManager->WriteStart();
}

ReflectRender::ReflectRender(const Vector2& _textureSize)
{
	s_pRenderTarget = std::make_shared<RenderTarget>(_textureSize);
	RenderManager::GetInstance()->Add(s_pRenderTarget);
}

void ReflectRender::Befoer()
{
	RenderManager::GetInstance()->WriteEnd();
	s_pRenderTarget->RBRenderTarget(true, true);
}

void ReflectRender::After()
{
	s_pRenderTarget->RBPixelShaderResource();
	RenderManager::GetInstance()->WriteStart();
}

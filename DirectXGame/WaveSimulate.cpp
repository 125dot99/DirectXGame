#include "WaveSimulate.h"

#include "math/MyMath.h"
#include "math/Quaternion.h"
#include "posteffect/RenderManager.h"
#include "pipeline/PipelineManager.h"

struct PaintCbData
{
	Vector4 pos;
	/*Vector4 scale;*/
	Vector2 screen;
	PaintCbData(const Vector3& pos, /*const Vector2& scale, */const Vector2& screen) :
		pos(pos), /*scale(scale),*/ screen(screen) {}
};

enum WAVE_TEX_INDEX
{
	INPUT_TEX,
	WAVE_PREV_TEX,
	WAVE_PREV_PREV_TEX,
	WAVE_TEX,
	NORMAL_TEX,
	REFLECT_TEX,
};

WaveSimulates::WaveSimulates(GameObject* pPaintObject, const Vector2& _textureSize, float meshSize)
{
	paintObject = pPaintObject;
	waveMeshSize = Vector2(meshSize, meshSize);

	//バッファの生成
	constBuffer.reset(new ConstBuffer);
	constBuffer->Init(0, sizeof(PaintCbData));
	for (int i = 0; i < REFLECT_TEX; i++)
	{
		if (i != NORMAL_TEX)
		{
			renderTarget[i].reset(new RenderTarget(_textureSize, DXGI_FORMAT_R32_FLOAT));
			RenderManager::GetInstance()->Add(renderTarget[i].get());
			continue;
		}
		renderTarget[i].reset(new RenderTarget(_textureSize));
		RenderManager::GetInstance()->Add(renderTarget[i].get());
	}
	for (int i = 0; i < 3; i++)
	{
		renderTarget[i + 1]->RBRenderTarget();
		PipelineManager::GetInstance()->GetPipelineState("WaveInitShader")->Command();
		RenderManager::GetInstance()->Draw();
		renderTarget[i + 1]->RBPixelShaderResource();
	}
}

void WaveSimulates::Befoer()
{

}

void WaveSimulates::After()
{
	RenderManager::GetInstance()->WriteEnd();
	const auto& pPipeline = PipelineManager::GetInstance();
	{
		const float s = 0.35f;
		Vector3 pos = paintObject->GetPosition();
		Vector2 rot = Vector2::FromAngle(paintObject->GetRotation().y);
		pos += Vector3(rot.y, 0, rot.x).Normalize() * s;
		pos.x *= 1.0f / waveMeshSize.x;
		pos.z *= 1.0f / waveMeshSize.y;
		pos += Vector3(0.5f, 0, 0.5f);
		pos.z = 1.0f - pos.z;
		PaintCbData p(pos, renderTarget[INPUT_TEX]->GetSize());
		constBuffer->Map(&p);
	}
	//0入力バッファ
	renderTarget[INPUT_TEX]->RBRenderTarget(true, false);
	pPipeline->GetPipelineState("WavePaintShader")->Command();
	constBuffer->GraphicsCommand();
	RenderManager::GetInstance()->Draw();
	renderTarget[INPUT_TEX]->RBPixelShaderResource();

	renderTarget[WAVE_PREV_PREV_TEX]->RBRenderTarget();
	pPipeline->GetPipelineState("PE_R32None")->Command();
	renderTarget[WAVE_PREV_TEX]->GraphicsSRVCommand(0);
	RenderManager::GetInstance()->Draw();
	renderTarget[WAVE_PREV_PREV_TEX]->RBPixelShaderResource();

	renderTarget[WAVE_PREV_TEX]->RBRenderTarget();
	pPipeline->GetPipelineState("PE_R32None")->Command();
	renderTarget[WAVE_TEX]->GraphicsSRVCommand(0);
	RenderManager::GetInstance()->Draw();
	renderTarget[WAVE_PREV_TEX]->RBPixelShaderResource();

	//合成
	renderTarget[WAVE_TEX]->RBRenderTarget();
	pPipeline->GetPipelineState("WaveSimulateShader")->Command();
	for (int i = 0; i < WAVE_TEX; i++)
	{
		renderTarget[i]->GraphicsSRVCommand(i);
	}
	RenderManager::GetInstance()->Draw();
	renderTarget[WAVE_TEX]->RBPixelShaderResource();

	//normal
	renderTarget[NORMAL_TEX]->RBRenderTarget();
	pPipeline->GetPipelineState("HighNormal")->Command();
	renderTarget[WAVE_TEX]->GraphicsSRVCommand(0);
	RenderManager::GetInstance()->Draw();
	renderTarget[NORMAL_TEX]->RBPixelShaderResource();
	RenderManager::GetInstance()->WriteStart();
}

ReflectRender::ReflectRender(const Vector2& _textureSize)
{
	render = std::make_unique<RenderTarget>(_textureSize);
	RenderManager::GetInstance()->Add(render.get());
}

void ReflectRender::Befoer()
{
	RenderManager::GetInstance()->WriteEnd();
	render->RBRenderTarget(true, true);
}

void ReflectRender::After()
{
	render->RBPixelShaderResource();
	RenderManager::GetInstance()->WriteStart();
}

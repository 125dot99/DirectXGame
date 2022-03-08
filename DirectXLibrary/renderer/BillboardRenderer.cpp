#include "BillboardRenderer.h"

#include "../pipeline/GraphicsPipeline.h"

static const int MAX_BILLBOARD = 100;

gamelib::BillboardRenderer::BillboardRenderer()
{
	objectCount = 0;
	u_pVertexBuffer = std::make_unique<VertexBuffer<VertexPosColorScaleAngle>>();
	u_pVertexBuffer->Create(MAX_BILLBOARD, MESH_PRIMITIVE::POINT_LIST);

	GraphicsPipelineDesc gpsoDesc;
	gpsoDesc.semantics = { "POSITION", "COLOR", "TEXCOORD", "PSIZE" };
	gpsoDesc.hlsl.vs.file = "ParticleVS.hlsl";
	gpsoDesc.hlsl.ps.file = "ParticlePS.hlsl";
	gpsoDesc.hlsl.gs.file = "ParticleGS.hlsl";
	gpsoDesc.primitive = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	gpsoDesc.cullMode = D3D12_CULL_MODE_NONE;
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD,  ROOT_PARAMETER::CAMERA, ROOT_PARAMETER::TEXTURE };
	u_pPipeline = std::make_unique<GraphicsPipeline>(gpsoDesc);
}

gamelib::BillboardRenderer::~BillboardRenderer()
{

}

void gamelib::BillboardRenderer::Begin()
{
	u_pPipeline->Command();
	u_pVertexBuffer->BufferCommand();
}

void gamelib::BillboardRenderer::Draw(const std::weak_ptr<Texture>& w_pTexture, const Vector3& pos, const Vector2& scale, const float rotate, const Vector4& color)
{
	if (objectCount >= MAX_BILLBOARD)
	{
		return;
	}
	static VertexPosColorScaleAngle vertex;
	const Vector2 size = w_pTexture.lock()->GetSize();
	float aspect = size.x / size.y;
	vertex = { pos, color, scale * Vector2(aspect, 1.0f), rotate };
	auto vertMap = ((VertexPosColorScaleAngle*)u_pVertexBuffer->GetMapPointer()) + objectCount;
	memcpy(vertMap, &vertex, sizeof(VertexPosColorScaleAngle));
	w_pTexture.lock()->GraphicsSRVCommand(2);
	u_pVertexBuffer->Draw(1, objectCount++);
}

void gamelib::BillboardRenderer::End()
{
	if (objectCount != 0)
	{
		objectCount = 0;
		u_pVertexBuffer->UnMap();
	}
}

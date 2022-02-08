#include "BillboardRenderer.h"

#include "../pipeline/GraphicsPipeline.h"

static const int MAX_BILLBOARD = 100;

gamelib::BillboardRenderer::BillboardRenderer()
{
	objectCount = 0;
	vertexBuffer = std::make_unique<VertexBuffer<VertexPosColorScaleAngle>>();
	vertexBuffer->Create(MAX_BILLBOARD, MESH_PRIMITIVE::POINT_LIST);

	GraphicsPipelineDesc gpsoDesc;
	gpsoDesc.semantics = { "POSITION", "COLOR", "TEXCOORD", "PSIZE" };
	gpsoDesc.hlsl.vs.file = "ParticleVS.hlsl";
	gpsoDesc.hlsl.ps.file = "ParticlePS.hlsl";
	gpsoDesc.hlsl.gs.file = "ParticleGS.hlsl";
	gpsoDesc.primitive = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	gpsoDesc.cullMode = D3D12_CULL_MODE_NONE;
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD,  ROOT_PARAMETER::CAMERA, ROOT_PARAMETER::TEXTURE };
	pipeline = std::make_unique<GraphicsPipeline>(gpsoDesc);
}

gamelib::BillboardRenderer::~BillboardRenderer()
{

}

void gamelib::BillboardRenderer::Begin()
{
	pipeline->Command();
	vertexBuffer->BufferCommand();
}

void gamelib::BillboardRenderer::Draw(const std::weak_ptr<Texture>& w_p_texture, const Vector3& pos, const Vector2& scale, const float rotate, const Vector4& color)
{
	if (objectCount >= MAX_BILLBOARD)
	{
		return;
	}
	static VertexPosColorScaleAngle vertex;
	const Vector2 size = w_p_texture.lock()->GetSize();
	float aspect = size.x / size.y;
	vertex = { pos, color, scale * Vector2(aspect, 1.0f), rotate };
	auto vertMap = ((VertexPosColorScaleAngle*)vertexBuffer->GetMapPointer()) + objectCount;
	memcpy(vertMap, &vertex, sizeof(VertexPosColorScaleAngle));
	w_p_texture.lock()->GraphicsSRVCommand(2);
	vertexBuffer->Draw(1, objectCount++);
}

void gamelib::BillboardRenderer::End()
{
	if (objectCount != 0)
	{
		objectCount = 0;
		vertexBuffer->UnMap();
	}
}

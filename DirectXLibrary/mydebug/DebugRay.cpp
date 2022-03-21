#include "DebugRay.h"

#include "../pipeline/GraphicsPipeline.h"

static const int MAX_RAY = 500;

gamelib::DebugRay::DebugRay()
{	
	drawCallCoount = 0;
	u_pVertexBuffer = std::make_unique<VertexBuffer<VertexLineSizeColor>>();
	u_pVertexBuffer->Create(MAX_RAY * 2, MESH_PRIMITIVE::LINE_LIST);
	
	GraphicsPipelineDesc gpsoDesc;
	gpsoDesc.semantics = { "POSITION", "COLOR", "PSIZE" };
	gpsoDesc.hlsl.vs.file = "LineVS.hlsl";
	gpsoDesc.hlsl.ps.file = "LinePS.hlsl";
	gpsoDesc.hlsl.gs.file = "LineGS.hlsl";
	gpsoDesc.primitive = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	gpsoDesc.cullMode = D3D12_CULL_MODE_NONE;
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD, ROOT_PARAMETER::CAMERA };
	u_pPipeline = std::make_unique<GraphicsPipeline>(gpsoDesc);
}

void gamelib::DebugRay::Draw(const Vector3& start, const Vector3& end, float length, const Vector4& color)
{
	if (drawCallCoount >= MAX_RAY)
	{
		return;
	}
	VertexLineSizeColor vertex[2] = { { start, color, length }, { end, color, length } };
	auto vertMap = (VertexLineSizeColor*)u_pVertexBuffer->GetMapPointer() + (UINT64)drawCallCoount++ * 2;
	memcpy(vertMap, vertex, sizeof(VertexLineSizeColor) * 2);
}

void gamelib::DebugRay::End()
{
	if (drawCallCoount == 0)
	{
		return;
	}
	u_pPipeline->Command();
	u_pVertexBuffer->UnMap();
	u_pVertexBuffer->BufferCommand();
	u_pVertexBuffer->Draw(drawCallCoount * 2, 0);
	drawCallCoount = 0;
}

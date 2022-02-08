#include "DebugRay.h"

#include "../pipeline/GraphicsPipeline.h"

static const int MAX_RAY = 500;

gamelib::DebugRay::DebugRay()
{	
	drawCallCoount = 0;
	vertexBuffer = std::make_unique<VertexBuffer<VertexLineSizeColor>>();
	vertexBuffer->Create(MAX_RAY * 2, MESH_PRIMITIVE::LINE_LIST);
	
	GraphicsPipelineDesc gpsoDesc;
	gpsoDesc.semantics = { "POSITION", "COLOR", "PSIZE" };
	gpsoDesc.hlsl.vs.file = "LineVS.hlsl";
	gpsoDesc.hlsl.ps.file = "LinePS.hlsl";
	gpsoDesc.hlsl.gs.file = "LineGS.hlsl";
	gpsoDesc.primitive = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	gpsoDesc.cullMode = D3D12_CULL_MODE_NONE;
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD, ROOT_PARAMETER::CAMERA };
	pipeline = std::make_unique<GraphicsPipeline>(gpsoDesc);
}

void gamelib::DebugRay::Draw(const Vector3& start, const Vector3& end, float length, const Vector4& color)
{
	if (drawCallCoount >= MAX_RAY)
	{
		return;
	}
	VertexLineSizeColor vertex[2] = { { start, color, length }, { end, color, length } };
	auto vertMap = (VertexLineSizeColor*)vertexBuffer->GetMapPointer() + (UINT64)drawCallCoount++ * 2;
	memcpy(vertMap, vertex, sizeof(VertexLineSizeColor) * 2);
}

void gamelib::DebugRay::End()
{
	if (drawCallCoount == 0)
	{
		return;
	}
	pipeline->Command();
	vertexBuffer->UnMap();
	vertexBuffer->BufferCommand();
	vertexBuffer->Draw(drawCallCoount * 2, 0);
	drawCallCoount = 0;
}

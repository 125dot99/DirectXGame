#include "GpuTerrainCollider.h"

#include "posteffect/RenderManager.h"
#include "pipeline/PipelineManager.h"
#include "imgui/imgui.h"

GpuTerrainCollider::GpuTerrainCollider()
{
	inputBuff = std::make_unique<ConstBuffer>();
	inputBuff->Init(0, sizeof(Vector3));
	heap = std::make_shared<DescriptorHeap>();
	heap->Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);
	uavBuffer = std::make_unique<RWStructuredBuffer>();
	uavBuffer->CreateUAV(sizeof(float), heap, 0);

	pipeline = PipelineManager::GetInstance()->GetPipelineState("CSTest");
}

void GpuTerrainCollider::Initialize(Texture* pHeightMap, const Vector2& _planeObjectScale)
{
	heightTexture = pHeightMap;
	Vector2 size = heightTexture->GetSize();
	halfSize = size / 2.0f;
	rectScale = halfSize / _planeObjectScale;
}

void GpuTerrainCollider::Command(GameObject* pObject)
{
	const Vector3& pos = pObject->GetPosition();
	Vector3 v = pos;
	v.x *= rectScale.x;
	v.z *= rectScale.y;
	v += Vector3(halfSize.x, 0, halfSize.y);
	v.z = halfSize.y * 2.0f - v.z;
	inputBuff->Map(&v);
	//ImGui::Begin("Debug");
	//{
	//	ImGui::Text("pos : %f, %f, %f", v.x, v.y, v.z);
	//}
	//ImGui::End();
	pipeline->Command();
	inputBuff->ComputeCommand();
	heightTexture->ComputeSRVCommand(1);
	uavBuffer->ComputeUAVCommand(2);
	uavBuffer->Dispatch(1, 1, 1);

	float* out = (float*)uavBuffer->GetCSOutput();
	pObject->SetPosition(Vector3(pos.x, *out, pos.z));
}

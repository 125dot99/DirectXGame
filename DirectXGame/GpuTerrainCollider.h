#pragma once
#include "math/Vector.h"
#include "dx12/Texture.h"
#include "dx12/DescriptorHeap.h"
#include "dx12/RWStructuredBuffer.h"
#include "dx12/ConstBuffer.h"
#include "pipeline/IPipelineState.h"
#include "object/GameObject.h"

using namespace gamelib;
class GpuTerrainCollider
{
private:
	Vector2 rectScale;
	Vector2 halfSize;

	std::shared_ptr<DescriptorHeap> descHeap;
	std::unique_ptr<ConstBuffer> inputBuff;
	std::unique_ptr<RWStructuredBuffer> uavBuffer;
	std::weak_ptr<Texture> heightTexture;
	std::weak_ptr<IPipelineState> pipeline;
public:
	GpuTerrainCollider();

	void Initialize(std::weak_ptr<Texture> pHeightMap, const Vector2& _planeObjectScale);

	void Command(GameObject* pObject);
};


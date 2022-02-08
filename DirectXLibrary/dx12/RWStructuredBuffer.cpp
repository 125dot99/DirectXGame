#include "RWStructuredBuffer.h"

#include <d3dx12.h>
#include <assert.h>
#include "Dx12Renderer.h"
#include "../math/MyMath.h"

void gamelib::RWStructuredBuffer::Map()
{
	CD3DX12_RANGE readRange(0, 1);
	resource->Map(0, &readRange, (void**)&pOutputData);
	resource->Unmap(0, nullptr);
}

gamelib::RWStructuredBuffer::RWStructuredBuffer() : index(), dataSize()
{

}

gamelib::RWStructuredBuffer::~RWStructuredBuffer()
{
}

void gamelib::RWStructuredBuffer::CreateUAV(const size_t& _size, std::weak_ptr<DescriptorHeap> pDescHeap, UINT index)
{
	this->index = index;
	dataSize = static_cast<UINT>(_size);
	auto uav = CD3DX12_RESOURCE_DESC::Buffer(dataSize);
	uav.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	//ライトバック
	auto memPropWB = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
	//ライトコンバイン
	auto memPropWC = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE, D3D12_MEMORY_POOL_L0);

	//UAVリソースの生成
	HRESULT result = Dx12Renderer::GetDevice()->CreateCommittedResource(
		&memPropWB,
		D3D12_HEAP_FLAG_NONE,
		&uav,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(&resource));

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.NumElements = 1;
	uavDesc.Buffer.StructureByteStride = dataSize;

	descriptorHeapUAV = pDescHeap;
	//デスクリプタヒープにUAV生成
	Dx12Renderer::GetDevice()->CreateUnorderedAccessView(
		resource.Get(), 
		nullptr, 
		&uavDesc, 
		pDescHeap.lock()->GetCPUHandle(index));
	Map();
}

void gamelib::RWStructuredBuffer::ComputeUAVCommand(UINT rootParamIndex)
{
	descriptorHeapUAV.lock()->Command();
	Dx12Renderer::GetCommandList()->SetComputeRootDescriptorTable(rootParamIndex, descriptorHeapUAV.lock()->GetGPUHandle(index));
}

void gamelib::RWStructuredBuffer::Dispatch(UINT x, UINT y, UINT z)
{
	Dx12Renderer::GetCommandList()->Dispatch(Min(1U, x), Min(1U, y), Min(1U, z));
}

void* gamelib::RWStructuredBuffer::GetCSOutput() const 
{
	return pOutputData;
}


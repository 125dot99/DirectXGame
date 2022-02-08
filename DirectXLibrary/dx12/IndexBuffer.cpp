#include "IndexBuffer.h"

#include <d3dx12.h>
#include <assert.h>

void gamelib::IndexBuffer::Create()
{
	indexSize = (UINT)indices.size();
	if (indexSize == 0 || resource)
	{
		return;
	}
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indexSize);
	HRESULT result = Dx12Renderer::GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result) && "インデックスバッファ生成失敗");

	Map();

	iBView.BufferLocation = resource->GetGPUVirtualAddress();
	iBView.Format = DXGI_FORMAT_R16_UINT;
	iBView.SizeInBytes = sizeIB;
}

void gamelib::IndexBuffer::Map()
{
	indexSize = (UINT)indices.size();
	unsigned short* indexMap = nullptr;
	HRESULT result = resource->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result) && "インデックスバッファ転送失敗");
	std::copy(indices.begin(), indices.end(), indexMap);
	resource->Unmap(0, nullptr);
}

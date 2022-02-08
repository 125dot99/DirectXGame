#include "DescriptorHeap.h"

#include <assert.h>

#include "Dx12Renderer.h"

void gamelib::DescriptorHeap::Create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT size)
{
	D3D12_DESCRIPTOR_HEAP_FLAGS flag = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	if (type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV || type == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER)
	{
		flag = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	}
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = type;
	descHeapDesc.NumDescriptors = size;
	descHeapDesc.Flags = flag;
	descHeapDesc.NodeMask = 0;
	HRESULT result = Dx12Renderer::GetDevice()->CreateDescriptorHeap(
		&descHeapDesc,
		IID_PPV_ARGS(&descHeap));
	assert(SUCCEEDED(result) && "デスクリプタヒープ生成失敗");
	//サイズをキャッシュする
	handleIncrementSize = Dx12Renderer::GetDevice()->GetDescriptorHandleIncrementSize(type);
}



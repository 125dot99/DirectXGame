#pragma once
#include <d3d12.h>
#include <wrl.h>

#include "Dx12Renderer.h"

namespace gamelib
{
//デスクリプタヒープのラッパークラス
class DescriptorHeap
{
private:
	UINT handleIncrementSize;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
public:
	/// <summary>
	/// デスクリプタヒープの生成
	/// </summary>
	/// <param name="type">定数バッファビュー(CBV)シェーダーリソースビュー(SRV)アクセスビュー(UAV)、レンダーターゲットビュー(RTV)、デプスステンシルビュー(DSV)</param>
	/// <param name="size">ヒープのサイズ</param>
	void Create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT size);
	
	/// <summary>
	/// デスクリプタヒープのセット
	/// </summary>
	inline void Command()
	{
		Dx12Renderer::GetCommandList()->SetDescriptorHeaps(1, descHeap.GetAddressOf());
	}
	
	/// <summary>
	/// 型を変換してアクセス可能
	/// </summary>
	inline operator ID3D12DescriptorHeap*() const
	{ 
		return descHeap.Get(); 
	}

	/// <summary>
	/// CPUハンドルを取得する
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	inline D3D12_CPU_DESCRIPTOR_HANDLE const GetCPUHandle(UINT index)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle(descHeap->GetCPUDescriptorHandleForHeapStart());
		handle.ptr = UINT64((INT64)handle.ptr + INT64(index) * INT64(handleIncrementSize));
		return handle;
	}

	/// <summary>
	/// GPUハンドルを取得する
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	inline D3D12_GPU_DESCRIPTOR_HANDLE const GetGPUHandle(UINT index)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE handle(descHeap->GetGPUDescriptorHandleForHeapStart());
		handle.ptr = UINT64((INT64)handle.ptr + INT64(index) * INT64(handleIncrementSize));
		return handle;
	}
};
} // namespace gamelib

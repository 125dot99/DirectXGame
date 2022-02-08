#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <assert.h>

#include "Dx12Renderer.h"

namespace gamelib
{
class ConstBuffer
{
private:
	//ルートパラメータの添え字
	UINT rootParamIndex;
	//構造体のサイズ
	UINT dataSize;
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
public:
	ConstBuffer() : rootParamIndex(0), dataSize(0) 
	{

	}

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(UINT rootParamIndex, size_t cbufferDataSize)
	{
		this->rootParamIndex = rootParamIndex;

		//ヒープ設定
		D3D12_HEAP_PROPERTIES cbheapprop{};
		cbheapprop.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用

		//リソース設定
		dataSize = static_cast<UINT>(cbufferDataSize);
		D3D12_RESOURCE_DESC cbresdesc{};
		cbresdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbresdesc.Width = (dataSize + 0xff) & ~0xff;//256バイトアライメント
		cbresdesc.Height = 1;
		cbresdesc.DepthOrArraySize = 1;
		cbresdesc.MipLevels = 1;
		cbresdesc.SampleDesc.Count = 1;
		cbresdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		//GPUリソースの生成
		HRESULT result = Dx12Renderer::GetDevice()->CreateCommittedResource(
			&cbheapprop,
			D3D12_HEAP_FLAG_NONE,
			&cbresdesc,//リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&resource));
		assert(SUCCEEDED(result) && "定数バッファの生成失敗");
	}

	/// <summary>
	/// GPUに定数バッファを転送
	/// </summary>
	inline void Map(void* cbuffer)
	{
		void* constMap = nullptr;
		HRESULT result = resource->Map(0, nullptr, (void**)&constMap);
		assert(SUCCEEDED(result) && "定数バッファの転送失敗");
		memcpy(constMap, cbuffer, dataSize);
		resource->Unmap(0, nullptr);
	}

	/// <summary>
	/// グラフィックスパイプラインのコマンド
	/// </summary>
	inline void GraphicsCommand() const
	{
		static auto* cmdList = Dx12Renderer::GetCommandList();
		cmdList->SetGraphicsRootConstantBufferView(rootParamIndex, resource->GetGPUVirtualAddress());
	}

	/// <summary>
	/// コンピュートパイプラインのコマンド
	/// </summary>
	inline void ComputeCommand() const
	{
		static auto* cmdList = Dx12Renderer::GetCommandList();
		cmdList->SetComputeRootConstantBufferView(rootParamIndex, resource->GetGPUVirtualAddress());
	}
};
} // namespace gamelib


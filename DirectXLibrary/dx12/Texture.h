#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <memory>

#include "../math/Vector.h"
#include "Dx12Renderer.h"
#include "DescriptorHeap.h"

namespace gamelib
{
class Texture
{
protected:
	//読み取った添え字
	UINT tableIndex;
	//テクスチャのサイズ
	Vector2 size;
	//ファイル名
	std::string name;
	//バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> com_pResource;
	//SRVのデスクリプタヒープ
	std::weak_ptr<DescriptorHeap> w_pDescriptorHeapSRV;
public:
	Texture() = default;
	virtual ~Texture() = default;
	Texture(ID3D12Resource* pResource, const std::string& _name);
	
	/// <summary>
	/// バッファ生成
	/// </summary>
	/// <param name="pDescHeap"></param>
	void CreateSRV(const std::weak_ptr<DescriptorHeap>& w_pDescriptorHeapSRV, UINT index);

	inline void GraphicsSRVCommand(UINT rootParamIndex) const
	{
		static auto* cmdList = Dx12Renderer::GetCommandList();
		w_pDescriptorHeapSRV.lock()->Command();
		cmdList->SetGraphicsRootDescriptorTable(rootParamIndex, w_pDescriptorHeapSRV.lock()->GetGPUHandle(tableIndex));
	}

	inline void ComputeSRVCommand(UINT rootParamIndex) const
	{
		static auto* cmdList = Dx12Renderer::GetCommandList();
		w_pDescriptorHeapSRV.lock()->Command();
		cmdList->SetComputeRootDescriptorTable(rootParamIndex, w_pDescriptorHeapSRV.lock()->GetGPUHandle(tableIndex));
	}

	/// <summary>
	/// 格納されているインデックスを取得
	/// </summary>
	/// <returns></returns>
	const UINT& GetSRVIndex() const;

	/// <summary>
	/// サイズを取得
	/// </summary>
	/// <returns></returns>
	inline const Vector2& GetSize() const
	{
		return size;
	}

	/// <summary>
	/// ファイル名取得
	/// </summary>
	/// <returns></returns>
	inline const std::string& GetName() const
	{
		return name;
	}
};
} // namespace gamelib

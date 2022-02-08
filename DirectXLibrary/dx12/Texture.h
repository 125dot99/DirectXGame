#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <memory>
#include "../math/Vector.h"
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
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	//SRVのデスクリプタヒープ
	std::weak_ptr<DescriptorHeap> descriptorHeapSRV;
public:
	Texture() = default;
	Texture(ID3D12Resource* pResource, const std::string& _name);
	
	/// <summary>
	/// バッファ生成
	/// </summary>
	/// <param name="pDescHeap"></param>
	void CreateSRV(std::weak_ptr<DescriptorHeap> pDescHeapSRV, UINT index);

	void GraphicsSRVCommand(UINT rootParamIndex) const;

	void ComputeSRVCommand(UINT rootParamIndex) const;

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

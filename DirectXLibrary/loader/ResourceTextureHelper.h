#pragma once
#include <d3d12.h>
#include <string>

#include "../dx12/GpuCopyPixels.h"

namespace gamelib
{
class ResourceTextureHelper
{
private:
	ResourceTextureHelper() = delete;
	~ResourceTextureHelper() = delete;

	static ID3D12Resource* LoadTexturePng(const std::string& fileName);
	
	static ID3D12Resource* LoadTextureBmp(const std::string& fileName);

	static ID3D12Resource* CreateTextureBuffer(const D3D12_RESOURCE_DESC& _resourceDesc);
public:
	/// <summary>
	/// テクスチャの読み取り
	/// </summary>
	/// <param name="_fileName"></param>
	/// <returns></returns>
	static ID3D12Resource* ReadTextureBuffer(const std::string& fileName);

	/// <summary>
	/// テクスチャの保存
	/// </summary>
	/// <param name="_fileName"></param>
	static void SaveTextureBuffer(const std::string& fileName, const MiniImage& image);
};
} // namespace gamelib

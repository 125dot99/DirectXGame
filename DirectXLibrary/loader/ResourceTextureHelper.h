#pragma once
#include <d3d12.h>
#include <string>

namespace gamelib
{
class ResourceTextureHelper
{
private:
	ResourceTextureHelper() = delete;
	~ResourceTextureHelper() = delete;

public:
	/// <summary>
	/// テクスチャの読み取り
	/// </summary>
	/// <param name="_fileName"></param>
	/// <returns></returns>
	static ID3D12Resource* ReadTextureBuffer(const std::string& _fileName);
};
} // namespace gamelib

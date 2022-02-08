#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>

#include "Dx12Renderer.h"

namespace gamelib
{
class IndexBuffer
{
private:
	//インデックスの要素数
	UINT indexSize;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW iBView{};
	//インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
public:
	//インデックス配列
	std::vector<unsigned short> indices;

	/// <summary>
	/// バッファを生成する
	/// </summary>
	void Create();
	
	/// <summary>
	/// バッファマッピング処理
	/// </summary>
	void Map();
	
	/// <summary>
	/// インデックスバッファのGPUコマンド
	/// </summary>
	inline void Draw()
	{
		static auto* cmdList = Dx12Renderer::GetCommandList();
		cmdList->IASetIndexBuffer(&iBView);
		cmdList->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
	}
};
} // namespace gamelib

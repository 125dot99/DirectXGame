#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include "DescriptorHeap.h"

namespace gamelib
{
//出力データを受け取るバッファを作成
class RWStructuredBuffer
{
private:
	//出力データのポインタ
	void* pOutputData;
	//デスクリプタヒープの格納されているインデックス
	UINT index;
	//出力データサイズ
	UINT dataSize;
	//バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> com_pResource;
	//UAVデスクリプタヒープ
	std::weak_ptr<DescriptorHeap> w_pDescriptorHeapUAV;

	void Map();
public:
	RWStructuredBuffer();

	/// <summary>
	/// UAVを生成
	/// </summary>
	/// <param name="_size"></param>
	/// <param name="pDescHeap"></param>
	/// <param name="index"></param>
	void CreateUAV(const size_t& _size, std::weak_ptr<DescriptorHeap> w_pDescriptorHeapUAV, UINT index);

	/// <summary>
	/// アンオーダードアクセスビューのコマンド送信
	/// </summary>
	/// <param name="rootParamIndex"></param>
	void ComputeUAVCommand(UINT rootParamIndex);
	
	/// <summary>
	/// コンピュートシェーダーの起動
	/// </summary>
	/// <param name="x">スレッドグループx</param>
	/// <param name="y">スレッドグループy</param>
	/// <param name="z">スレッドグループz</param>
	void Dispatch(UINT x, UINT y, UINT z);
	
	/// <summary>
	/// 書き込んだバッファを取得
	/// </summary>
	/// <returns></returns>
	void* GetCSOutput() const;
};
} // namespace gamelib

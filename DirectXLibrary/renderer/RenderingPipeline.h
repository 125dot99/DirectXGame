#pragma once
#include <vector>
#include <memory>
#include <unordered_map>

#include "../pipeline/IPipelineState.h"
#include "../renderer/MeshRenderer.h"
#include "../dx12/RenderTarget.h"

namespace gamelib
{
//全ての描画システムを組込む
class RenderingPipeline
{
private:
	//シェーダー
	std::unordered_map<UINT, std::weak_ptr<IPipelineState>> u_mapPipelines;
	//描画オブジェクト
	std::unordered_map<UINT, std::vector<std::shared_ptr<MeshRenderer>>> u_mapRenderings;
public:
	RenderingPipeline();
	
	/// <summary>
	/// パイプラインを設定する
	/// </summary>
	/// <param name="index"></param>
	/// <param name="shaderName"></param>
	void SetPipeline(UINT index, const std::string& shaderName);

	/// <summary>
	/// 描画オブジェクトを追加
	/// </summary>
	/// <param name="index"></param>
	/// <param name="renderer"></param>
	void Add(UINT index, std::shared_ptr<MeshRenderer> renderer);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void Begin();
	
	/// <summary>
	/// 全て描画
	/// </summary>
	void Draw();
};
} // namespace gamelib

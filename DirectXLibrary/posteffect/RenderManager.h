#pragma once
#include "../dx12/RenderTarget.h"
#include "../dx12/VertexBuffer.h"
#include <vector>

namespace gamelib
{
class RenderManager
{
private:
	std::unique_ptr<RenderTarget> sourceTexture;
	std::unique_ptr<VertexBuffer<VertexUv>> vertexBuffer;
	std::shared_ptr<DescriptorHeap> descriptorHeapSRV;
	std::shared_ptr<DescriptorHeap> descriptorHeapRTV;

	//レンダーテクスチャの管理
	std::vector<RenderTarget*> textures;

	RenderManager();
	~RenderManager() = default;
	RenderManager(const RenderManager&) = delete;
	void operator=(const RenderManager&) = delete;
public:
	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static RenderManager* GetInstance();

	/// <summary>
	/// レンダーテクスチャの作成、読み取り用テクスチャの追加
	/// </summary>
	/// <param name="postEffect"></param>
	void Add(RenderTarget* pRenderTarget);

	/// <summary>
	/// レンダーテクスチャの作成
	/// </summary>
	/// <param name="pRenderTarget"></param>
	/// <param name="index"></param>
	void Create(RenderTarget* pRenderTarget, UINT index);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void WriteStart(bool isClear = true, bool isDepth = true);

	/// <summary>
	/// 描画後処理
	/// </summary>
	void WriteEnd();

	/// <summary>
	/// 最終的な描画結果
	/// </summary>
	void Result();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 削除
	/// </summary>
	void Clear();

	/// <summary>
	/// テクスチャを取得
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	Texture* GetRenderTarget(int index) const;
};
} // namespace gamelib


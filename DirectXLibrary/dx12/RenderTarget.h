#pragma once
#include "Texture.h"

namespace gamelib
{
//レンダーテクスチャクラス
class RenderTexture : public Texture
{
public:
	friend class RenderTarget;
	RenderTexture(const Vector2& _textureSize, DXGI_FORMAT format);
};

class RenderTarget : public Texture
{
private:
	//RTVのデスクリプタヒープ
	std::weak_ptr<DescriptorHeap> descriptorHeapRTV;
	//描画矩形コマンド送信
	void RasterizerStageCommand();
	//レンダーターゲットビューコマンド送信
	void OMSetRenderTarget();
	void OMSetRenderTargetClear();
	//レンダーターゲットビューとデプスステンシルビューコマンド送信
	void OMSetRenderTargetDepth();
	void OMSetRenderTargetDepthClear();
public:
	RenderTarget(const Vector2& textureSize, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	/// <summary>
	/// レンダーターゲットビューの作成
	/// </summary>
	/// <param name="pDescHeapRTV"></param>
	void CreateRTV(std::weak_ptr<DescriptorHeap> pDescHeapRTV);

	/// <summary>
	/// レンダーターゲットに設定
	/// </summary>
	/// <param name="isClear">画面をクリアするか</param>
	/// <param name="isDepth">深度ステンシルをセットするか</param>
	void RBRenderTarget(bool isClear = false, bool isDepth = false);

	/// <summary>
	/// ピクセルシェーダーリソースに設定
	/// </summary>
	void RBPixelShaderResource();
};
} // namespace gamelib


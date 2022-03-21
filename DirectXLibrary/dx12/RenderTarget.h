#pragma once
#include "Texture.h"

namespace gamelib
{
class RenderTarget : public Texture
{
private:
	float clearColors[4];
	//RTVのデスクリプタヒープ
	std::weak_ptr<DescriptorHeap> w_pDescriptorHeapRTV;
public:
	RenderTarget(const Vector2& textureSize, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, const Vector4& clearColor = {});

	/// <summary>
	/// レンダーターゲットビューの作成
	/// </summary>
	/// <param name="pDescHeapRTV"></param>
	void CreateRTV(const std::weak_ptr<DescriptorHeap>& w_pDescriptorHeapRTV);

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


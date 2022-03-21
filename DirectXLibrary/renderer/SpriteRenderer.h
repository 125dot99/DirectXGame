#pragma once
#include "../dx12/Texture.h"
#include "../math/Vector.h"
#include "../dx12/VertexBuffer.h"
#include "../dx12/ConstBuffer.h"
#include "../pipeline/IPipelineState.h"
#include <memory>

namespace gamelib
{
class SpriteRenderer
{
private:
	//スプライトの数
	int spriteCount;
	//頂点バッファ
	std::unique_ptr<VertexBuffer<VertexSprite>> u_pVertexBuffer;
	//定数バッファ
	std::unique_ptr<ConstBuffer> u_pConstBuffer;
	//スプライトのパイプライン
	std::unique_ptr<IPipelineState> u_pPipeline;
	//スプライトの描画
	void Renderer(
		const std::weak_ptr<Texture>& w_pTexture,
		const Vector2& pos, 
		const Vector2& size, 
		float angle, 
		const Vector2& pivot, 
		const Vector4& rect, 
		const Vector4& color);
public:
	SpriteRenderer();

	/// <summary>
	/// スプライトの描画前準備
	/// </summary>
	void Begin();

	void SetScreenSize(const Vector2& screenSize);

	//通常
	void Draw(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector4& color = { 1,1,1,1 });
	void Draw(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& pivot, const Vector4& color = { 1,1,1,1 });
	
	//左右反転
	void DrawTurnX(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector4& color = { 1,1,1,1 });
	
	//上下反転
	void DrawTurnY(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector4& color = { 1,1,1,1 });

	//サイズ指定
	void DrawExtend(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& size, const Vector4& color = { 1,1,1,1 });
	void DrawExtend(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& size, const Vector2& pivot, const Vector4& color = { 1,1,1,1 });

	//回転
	void DrawRotate(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, float angle, const Vector4& color = { 1,1,1,1 });
	void DrawRotate(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, float angle, const Vector2& pivot, const Vector4& color = { 1,1,1,1 });
	
	//指定した矩形部分のみ描画
	void DrawRect(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector4& rect, const Vector4& color = { 1,1,1,1 });
	
	//拡大縮小、指定した矩形部分のみ描画
	void DrawExtendRect(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& size, const Vector4& rect, const Vector4& color = { 1,1,1,1 });
	void DrawExtendRect(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& size, const Vector4& rect, const Vector2& pivot, const Vector4& color = { 1,1,1,1 });
	
	void DrawExtendMakeRect(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& size, const Vector2& indexSize, int index, const Vector4& color = { 1,1,1,1 });

	/// <summary>
	/// スプライトの描画後始末
	/// </summary>
	void End();
};
} // namespace gamelib


#pragma once
namespace gamelib
{
/// <summary>
/// メッシュインターフェース
/// </summary>
struct IMesh abstract
{
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IMesh() = default;

	/// <summary>
	/// バッファ生成
	/// </summary>
	virtual void CreateBuffers() = 0;

	/// <summary>
	/// 描画コマンド
	/// </summary>
	virtual void Draw() = 0;
};
} // namespace gamelib

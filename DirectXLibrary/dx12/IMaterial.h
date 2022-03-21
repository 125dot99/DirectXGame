#pragma once
namespace gamelib
{
/// <summary>
/// マテリアルインターフェース
/// </summary>
struct IMaterial abstract
{
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IMaterial() = default;

	/// <summary>
	/// バッファ生成
	/// </summary>
	virtual void Create() = 0;

	/// <summary>
	/// マテリアルのGPUコマンド
	/// </summary>
	virtual void Register() = 0;
};
} // namespace gamelib

#pragma once
namespace gamelib
{
class IPostProcess
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IPostProcess() = default;

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	/// <param name="pPost"></param>
	virtual void Befoer() = 0;

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	/// <param name="pPost"></param>
	virtual void After() = 0;
};
} // namespace gamelib
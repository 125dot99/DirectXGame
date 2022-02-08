#pragma once
namespace gamelib
{
class HitPoint final
{
private:
	float hp;
	float max;
public:
	/// <summary>
	/// 体力を設定
	/// </summary>
	/// <param name="point"></param>
	void SetHP(const float point);

	/// <summary>
	/// 体力を回復する
	/// </summary>
	/// <param name="point">回復量</param>
	void Recovery(const float point);

	/// <summary>
	/// ダメージを受ける
	/// </summary>
	/// <param name="point">ダメージ量</param>
	void Damage(const float point);

	/// <summary>
	/// 体力を取得
	/// </summary>
	/// <param name="point"></param>
	const float& GetHP() const;

	/// <summary>
	/// 表示用の体力ゲージの値を返す
	/// </summary>
	/// <param name="spriteSize"></param>
	/// <returns></returns>
	float GetLifeGage(const float spriteSize) const;

	/// <summary>
	/// 体力は残っているか
	/// </summary>
	/// <returns></returns>
	bool IsDead() const;
};
} // namespace gamelib


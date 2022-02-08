#pragma once
#include "../math/Vector.h"

namespace gamelib
{
enum class MOUSE_CODE
{
	LEFT,
	RIGHT,
	CENTOR
};

//マウスインターフェース
class IMouse
{
public:
	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IMouse() = default;

	/// <summary>
	/// 毎フレーム更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 押しているか判定する
	/// </summary>
	/// <param name="buttonNum"></param>
	/// <returns>押している間tureを返す</returns>
	virtual bool IsButton(MOUSE_CODE mouseCode) const = 0;

	/// <summary>
	/// 押した瞬間の判定する
	/// </summary>
	/// <param name="buttonNum"></param>
	/// <returns>押した瞬間tureを返す</returns>
	virtual bool IsButtonDown(MOUSE_CODE mouseCode) const = 0;

	/// <summary>
	/// 離した瞬間の判定する
	/// </summary>
	/// <param name="buttonNum"></param>
	/// <returns>離した瞬間tureを返す</returns>
	virtual bool IsButtonUp(MOUSE_CODE mouseCode) const = 0;

	/// <summary>
	/// ホイールの回転値を取得する
	/// </summary>
	/// <returns>ホイールの回転値</returns>
	virtual float GetWheel() const = 0;

	/// <summary>
	/// マウスカーソルの位置を取得する
	/// </summary>
	/// <returns>マウスの位置</returns>
	virtual const Vector2& GetPoint() const = 0;

	/// <summary>
	/// 1フレームのマウスの移動量を取得する
	/// </summary>
	/// <returns>マウスの移動量</returns>
	virtual Vector2 GetVelocity() const = 0;
};
} // namespace gamelib
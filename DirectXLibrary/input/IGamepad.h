#pragma once
#include "../math/Vector.h"

namespace gamelib
{
//入力ボタン定義
enum class GAMEPAD_CODE
{
	X,				//Xボタン
	Y,				//Yボタン
	A,				//Aボタン
	B,				//Bボタン
	LEFT_SHOULDER,	//Lボタン
	RIGHT_SHOULDER,	//Rボタン
	LEFT_TRIGGER,	//Lトリガー
	RIGHT_TRIGGER,	//Rトリガー
	LEFT_THUMB,		//Lスティック押し込み
	RIGHT_THUMB,	//Rスティック押し込み
	BACK,			//Backボタン
	START,			//Startボタン
	UP,				//十字ボタン↑
	DOWN,			//十字ボタン↓
	LEFT,			//十字ボタン←
	RIGHT,			//十字ボタン→
};

//ゲームパッドデバイスインターフェース
class IGamepad
{
public:
	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IGamepad() = default;
	
	/// <summary>
	/// ゲームパッド初期化
	/// </summary>
	/// <returns>デバイスを生成出来たか</returns>
	virtual bool Initialize() = 0;
	
	/// <summary>
	/// 毎フレーム更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 押しているか判定する
	/// </summary>
	/// <param name="buttonNum"></param>
	/// <returns>押している間tureを返す</returns>
	virtual bool IsButton(GAMEPAD_CODE buttonCode) const = 0;

	/// <summary>
	/// 押した瞬間の判定する
	/// </summary>
	/// <param name="buttonNum"></param>
	/// <returns>押した瞬間tureを返す</returns>
	virtual bool IsButtonDown(GAMEPAD_CODE buttonCode) const = 0;

	/// <summary>
	/// 離した瞬間の判定する
	/// </summary>
	/// <param name="buttonNum"></param>
	/// <returns>離した瞬間tureを返す</returns>
	virtual bool IsButtonUp(GAMEPAD_CODE buttonCode) const = 0;
	
	/// <summary>
	/// 左スティックの傾きを取得する
	/// </summary>
	/// <returns>左スティックの入力値</returns>
	virtual const Vector2& GetLeftThumb() const = 0;

	/// <summary>
	/// 右スティックの傾きを取得する
	/// </summary>
	/// <returns>右スティックの入力値</returns>
	virtual const Vector2& GetRightThumb() const = 0;

	/// <summary>
	/// コントローラー左側の振動の設定
	/// </summary>
	/// <param name="power">振動する強さ</param>
	virtual void SetLeftVibration(int power) = 0;
	
	/// <summary>
	/// コントローラー右側の振動の設定
	/// </summary>
	/// <param name="power">振動する強さ</param>
	virtual void SetRightVibration(int power) = 0;
};
} // namespace gamelib

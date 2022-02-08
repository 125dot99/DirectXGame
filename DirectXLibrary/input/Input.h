#pragma once
#include "IKeyborad.h"
#include "IMouse.h"
#include "IGamepad.h"
#include <memory>

namespace gamelib
{
class Input
{
private:
	static std::unique_ptr<IKeyboard> keyboard;
	static std::unique_ptr<IMouse> mouse;
	static std::unique_ptr<IGamepad> gamepad;
public:
	/// <summary>
	/// 入力デバイス初期化処理
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 毎フレーム処理 デバイスの入力情報を更新
	/// </summary>
	static void Update();

	/// <summary>
	/// キーボードの入力
	/// </summary>
	/// <returns>キーボードのポインタ</returns>
	static inline IKeyboard* GetKeyborad() { return keyboard.get(); }
	
	/// <summary>
	/// マウスの入力
	/// </summary>
	/// <returns>マウスのポインタ</returns>
	static inline IMouse* GetMouse() { return mouse.get(); }

	/// <summary>
	/// ゲームパッドの入力
	/// </summary>
	/// <returns>ゲームパッドのポインタ</returns>
	static inline IGamepad* GetGamepad() { return gamepad.get(); }
};
} // namespace gamelib


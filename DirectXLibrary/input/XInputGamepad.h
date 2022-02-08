#pragma once
#include <Windows.h>
#include <Xinput.h>

#include "IGamepad.h"
#pragma comment(lib, "xinput.lib")

namespace gamelib
{
class XInputGamepad : public IGamepad
{
private:
	//XInputのコード判別用
	enum XINPUT_BUTTON_ENUM
	{
		XINPUT_LTRREGER = 6,
		XINPUT_RTRREGER = 7,
	};
	//ビット変換配列
	static const int MAX_BUTTON = 16;
	static unsigned int xButtons[MAX_BUTTON];
	//スティックの入力値
	Vector2 leftThumb = {};
	Vector2 rightThumb = {};
	//XInputの入力構造体
	XINPUT_STATE state = {};
	//前フレームの入力値格納
	XINPUT_GAMEPAD prevGamepad = {};
	//振動を使う
	XINPUT_VIBRATION vibration = {};
	//DirectInputのボタンと合わせるため変換
	void SetButtonArray();
	//トリガー入力判定
	bool IsTriggerRangeNum(int num) const;
	bool IsTrigger(bool isLeft) const;
	bool IsTriggerDown(bool isLeft) const;
	bool IsTriggerUp(bool isLeft) const;
public:
	bool Initialize() override;
	void Update() override;
	bool IsButton(GAMEPAD_CODE buttonCode) const override;
	bool IsButtonDown(GAMEPAD_CODE buttonCode) const override;
	bool IsButtonUp(GAMEPAD_CODE buttonCode) const override;
	inline const Vector2& GetLeftThumb() const override { return leftThumb; }
	inline const Vector2& GetRightThumb() const override { return rightThumb; }
	void SetLeftVibration(int power) override;
	void SetRightVibration(int power) override;
};
} // namespace gamelib
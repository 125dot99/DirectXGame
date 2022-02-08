#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <wrl.h>
#include "IGamepad.h"

namespace gamelib
{
class DirectInputGamepad : public IGamepad
{
private:
	//DirectXInputのコード判別用
	enum DIERCTINPUT_BUTTON_ENUM
	{
		DIRECTINPUT_UP = 12,
		DIRECTINPUT_DOWN,
		DIRECTINPUT_LEFT,
		DIRECTINPUT_RIGHT,
	};
	//スティック入力最大値
	static const unsigned short MAX_STICK_VALUE = 0x8000;
	//スティックの入力値
	Vector2 leftThumb = {};
	Vector2 rightThumb = {};
	//DirectInputの入力構造体
	DIJOYSTATE padState = {};
	//前フレームの入力値格納
	DIJOYSTATE padStatePre = {};
	//DirectInputの入力を受け取るデバイス
	Microsoft::WRL::ComPtr<IDirectInputDevice8W> dInputDevice;
	//十字ボタンの入力
	bool IsDpad(unsigned int value) const;
	bool IsDpadDown(unsigned int value) const;
	bool IsDpadUp(unsigned int value) const;
	unsigned int ConvertDirButton(int num) const;
public:
	bool Initialize() override;
	void Update() override;
	bool IsButton(GAMEPAD_CODE buttonCode) const override;
	bool IsButtonDown(GAMEPAD_CODE buttonCode) const override;
	bool IsButtonUp(GAMEPAD_CODE buttonCode) const override;
	inline const Vector2& GetLeftThumb() const override { return leftThumb; }
	inline const Vector2& GetRightThumb() const override { return rightThumb; }
	inline void SetLeftVibration(int power) override {}
	inline void SetRightVibration(int power) override {}
};
} // namespace gamelib
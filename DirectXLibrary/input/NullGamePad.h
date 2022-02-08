#pragma once
#include "IGamepad.h"

namespace gamelib
{
//取得失敗時のゲームパッド
class NullGamePad : public IGamepad
{
private:
	const Vector2 ZERO = Vector2::Zero();
public:
	inline bool Initialize() override { return false; }
	inline void Update() override {}
	inline bool IsButton(GAMEPAD_CODE buttonCode) const override { return false; }
	inline bool IsButtonDown(GAMEPAD_CODE buttonCode) const override { return false; }
	inline bool IsButtonUp(GAMEPAD_CODE buttonCode) const override { return false; }
	inline const Vector2& GetLeftThumb() const override { return ZERO; }
	inline const Vector2& GetRightThumb() const override { return ZERO; }
	inline void SetLeftVibration(int power) override {}
	inline void SetRightVibration(int power) override {}
};
} // namespace gamelib
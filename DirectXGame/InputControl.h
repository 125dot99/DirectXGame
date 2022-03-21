#pragma once
#include "input/Input.h"

using namespace gamelib;

//“ü—Íˆ—
namespace input_control
{
inline const Vector2 LeftStickValue()
{
	auto& leftThomb = Input::GetGamepad()->GetLeftThumb();
	Vector2 v;
	if (Input::GetKeyborad()->IsKey(KEY_CODE::D) ||
		leftThomb.x > 0)
	{
		v.x += 1;
	}
	if (Input::GetKeyborad()->IsKey(KEY_CODE::A) ||
		leftThomb.x < 0)
	{
		v.x -= 1;
	}
	if (Input::GetKeyborad()->IsKey(KEY_CODE::W) ||
		leftThomb.y > 0)
	{
		v.y += 1;
	}
	if (Input::GetKeyborad()->IsKey(KEY_CODE::S) ||
		leftThomb.y < 0)
	{
		v.y -= 1;
	}
	return v;
}

inline const bool MoveAction()
{
	return Input::GetKeyborad()->IsKey(KEY_CODE::D) ||
		Input::GetKeyborad()->IsKey(KEY_CODE::A) ||
		Input::GetKeyborad()->IsKey(KEY_CODE::W) ||
		Input::GetKeyborad()->IsKey(KEY_CODE::S) ||
		Input::GetGamepad()->GetLeftThumb().Length() > 0;
}

inline const bool MainAction()
{
	return Input::GetKeyborad()->IsKeyDown(KEY_CODE::SPACE) ||
		Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::B);
}

inline const bool SubAction()
{
	return Input::GetKeyborad()->IsKeyDown(KEY_CODE::X) ||
		Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::X);
}

inline const bool CancelAction()
{
	return Input::GetKeyborad()->IsKeyDown(KEY_CODE::Z) ||
		Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::A);
}

inline const bool PauseFilterLeft()
{
	return Input::GetKeyborad()->IsKeyDown(KEY_CODE::_1) ||
		Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::LEFT);
}

inline const bool PauseFilterRight()
{
	return Input::GetKeyborad()->IsKeyDown(KEY_CODE::_2) ||
		Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::RIGHT);
}
} // namespace input_control

#include "XInputGamepad.h"

unsigned int gamelib::XInputGamepad::xButtons[MAX_BUTTON] = {};

void gamelib::XInputGamepad::SetButtonArray()
{
	static bool init = false;
	if (init) return;
	unsigned int buttons[MAX_BUTTON] =
	{
		XINPUT_GAMEPAD_X,
		XINPUT_GAMEPAD_Y,
		XINPUT_GAMEPAD_A,
		XINPUT_GAMEPAD_B,
		XINPUT_GAMEPAD_LEFT_SHOULDER,
		XINPUT_GAMEPAD_RIGHT_SHOULDER,
		XINPUT_LTRREGER,
		XINPUT_RTRREGER,
		XINPUT_GAMEPAD_LEFT_THUMB,
		XINPUT_GAMEPAD_RIGHT_THUMB,
		XINPUT_GAMEPAD_BACK,
		XINPUT_GAMEPAD_START,
		XINPUT_GAMEPAD_DPAD_UP,
		XINPUT_GAMEPAD_DPAD_DOWN,
		XINPUT_GAMEPAD_DPAD_LEFT,
		XINPUT_GAMEPAD_DPAD_RIGHT,
	};
	memcpy(xButtons, buttons, sizeof(buttons));
	init = true;
}

bool gamelib::XInputGamepad::IsTriggerRangeNum(int num) const
{
	return num == XINPUT_LTRREGER || num == XINPUT_RTRREGER;
}

bool gamelib::XInputGamepad::IsTrigger(bool isLeft) const
{
	if (isLeft)
	{
		return state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
	}
	return state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
}

bool gamelib::XInputGamepad::IsTriggerDown(bool isLeft) const
{
	if (isLeft)
	{
		return state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD &&
			!(prevGamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}
	return state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD &&
		!(prevGamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
}

bool gamelib::XInputGamepad::IsTriggerUp(bool isLeft) const
{
	if (isLeft)
	{
		return !(state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) &&
			prevGamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
	}
	return !(state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) &&
		prevGamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
}

bool gamelib::XInputGamepad::Initialize()
{
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	DWORD dwResult = XInputGetState(0, &state);
	if (dwResult != ERROR_SUCCESS)
	{
		return false;
	}
	OutputDebugStringA("XInputゲームパッドが見つかりました\n");
	SetButtonArray();
	return true;
}

void gamelib::XInputGamepad::Update()
{
	memcpy(&prevGamepad, &state.Gamepad, sizeof(XINPUT_GAMEPAD));
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	DWORD dwResult = XInputGetState(0, &state);
	if (dwResult != ERROR_SUCCESS)
	{
		leftThumb = rightThumb = Vector2::Zero();
		return;
	}
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	XInputSetState(0, &vibration);

	leftThumb.x = static_cast<float>(state.Gamepad.sThumbLX);
	leftThumb.y = static_cast<float>(state.Gamepad.sThumbLY);
	rightThumb.x = static_cast<float>(state.Gamepad.sThumbRX);
	rightThumb.y = static_cast<float>(state.Gamepad.sThumbRY);
	leftThumb.Normalize();
	rightThumb.Normalize();
}

bool gamelib::XInputGamepad::IsButton(GAMEPAD_CODE buttonCode) const
{
	int buttonNum = static_cast<int>(buttonCode);
	if (IsTriggerRangeNum(buttonNum))
	{
		return IsTrigger(buttonNum == XINPUT_LTRREGER);
	}
	return state.Gamepad.wButtons & xButtons[buttonNum];
}

bool gamelib::XInputGamepad::IsButtonDown(GAMEPAD_CODE buttonCode) const
{
	int buttonNum = static_cast<int>(buttonCode);
	if (IsTriggerRangeNum(buttonNum))
	{
		return IsTriggerDown(buttonNum == XINPUT_LTRREGER);
	}
	return state.Gamepad.wButtons & xButtons[buttonNum] &&
		!(prevGamepad.wButtons & xButtons[buttonNum]);
}

bool gamelib::XInputGamepad::IsButtonUp(GAMEPAD_CODE buttonCode) const
{
	int buttonNum = static_cast<int>(buttonCode);
	if (IsTriggerRangeNum(buttonNum))
	{
		return IsTriggerUp(buttonNum == XINPUT_LTRREGER);
	}
	return !(state.Gamepad.wButtons & xButtons[buttonNum]) &&
		prevGamepad.wButtons & xButtons[buttonNum];
}

void gamelib::XInputGamepad::SetLeftVibration(int power)
{
	vibration.wLeftMotorSpeed = power;
	XInputSetState(0, &vibration);
}

void gamelib::XInputGamepad::SetRightVibration(int power)
{
	vibration.wRightMotorSpeed = power;
	XInputSetState(0, &vibration);
}

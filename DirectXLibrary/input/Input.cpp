#include "Input.h"

#include "Mouse.h"
#include "Keyboard.h"

#include "NullGamePad.h"
#include "DirectInputGamepad.h"
#include "XInputGamepad.h"

namespace gamelib
{
std::unique_ptr<IKeyboard> Input::keyboard;
std::unique_ptr<IMouse> Input::mouse;
std::unique_ptr<IGamepad> Input::gamepad;
} // namespace gamelib

void gamelib::Input::Initialize()
{
	keyboard = std::make_unique<Keyboard>();
	mouse = std::make_unique<Mouse>();

	//ゲームパッドデバイス取得
	gamepad = std::make_unique<DirectInputGamepad>();
	if (gamepad->Initialize())
	{
		return;
	}
	gamepad = std::make_unique<XInputGamepad>();
	if (gamepad->Initialize())
	{
		return;
	}
	gamepad = std::make_unique<NullGamePad>();
	OutputDebugStringA("ゲームパッドが見つかりませんでした\n");
}

void gamelib::Input::Update()
{
	keyboard->Update();
	mouse->Update();
	gamepad->Update();
}


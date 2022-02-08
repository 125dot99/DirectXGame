#include "DirectInputGamepad.h"

#include <assert.h>
#include <vector>

#include "../app/Application.h"

//デバイス列挙するコールバック関数
BOOL CALLBACK DIEnumDeviceCallback(LPDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	std::vector<DIDEVICEINSTANCE>* lpddis = (std::vector<DIDEVICEINSTANCE>*)pvRef;
	lpddis->emplace_back(*lpddi);//ゲームパッドデバイスを格納
	return DIENUM_CONTINUE;	//次のデバイスを列挙
}

//デバイスのオブジェクトを列挙するコールバック関数
BOOL CALLBACK DIEnumDeviceObjectCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	std::vector<DIDEVICEOBJECTINSTANCE>* lpddois = (std::vector<DIDEVICEOBJECTINSTANCE>*)pvRef;
	lpddois->emplace_back(*lpddoi);//ゲームパッドデバイスオブジェクトを格納
	return DIENUM_CONTINUE;	//次のデバイスを列挙
}

bool gamelib::DirectInputGamepad::IsDpad(unsigned int value) const
{
	static const unsigned int DIR = 4500;
	if (value == 0)
	{
		return (padState.rgdwPOV[0] >= value && padState.rgdwPOV[0] <= value + DIR ||
			padState.rgdwPOV[0] >= 31500);
	}
	return (padState.rgdwPOV[0] >= value - DIR && padState.rgdwPOV[0] <= value + DIR);
}

bool gamelib::DirectInputGamepad::IsDpadDown(unsigned int value) const
{
	return (padState.rgdwPOV[0] == value &&
		padStatePre.rgdwPOV[0] != value);
}

bool gamelib::DirectInputGamepad::IsDpadUp(unsigned int value) const
{
	return (padState.rgdwPOV[0] != value &&
		padStatePre.rgdwPOV[0] == value);
}

unsigned int gamelib::DirectInputGamepad::ConvertDirButton(int num) const
{
	enum { UP = 0, RIGHT = 9000, DOWN = 18000, LEFT = 27000, MAX = 36000 };
	switch (num)
	{
	case DIRECTINPUT_UP: 
		return UP;
	case DIRECTINPUT_DOWN: 
		return DOWN;
	case DIRECTINPUT_RIGHT: 
		return RIGHT;
	case DIRECTINPUT_LEFT: 
		return LEFT;
	default: break;
	}
	return MAX;//無効範囲
}

bool gamelib::DirectInputGamepad::Initialize()
{
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	Microsoft::WRL::ComPtr<IDirectInput8W> dinput;
	HRESULT result = DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		nullptr);
	assert(SUCCEEDED(result));

	//デバイスの列挙を取得
	std::vector<DIDEVICEINSTANCE> deviceArray;
	//ジョイパッドを探す
	result = dinput->EnumDevices(
		DI8DEVTYPE_JOYSTICK,//DI8DEVCLASS_GAMECTRL,
		(LPDIENUMDEVICESCALLBACK)DIEnumDeviceCallback,
		&deviceArray,
		DIEDFL_ATTACHEDONLY);
	const int INDEX = 0;
	if (deviceArray.size() <= INDEX)//DirectInputゲームパッド取得失敗
	{
		ZeroMemory(&deviceArray, sizeof(deviceArray));
		return false;
	}
	OutputDebugStringA("DirectInputゲームパッドが見つかりました\n");
	//ここから初期化処理
	result = dinput->CreateDevice(
		deviceArray[INDEX].guidInstance,
		dInputDevice.ReleaseAndGetAddressOf(),
		nullptr);

	assert(SUCCEEDED(result) && "デバイス生成失敗");
	ZeroMemory(&deviceArray, sizeof(deviceArray));

	// ジョイスティック用のデータ・フォーマットを設定
	result = dInputDevice->SetDataFormat(&c_dfDIJoystick);
	assert(SUCCEEDED(result));

	// モードを設定（フォアグラウンド＆非排他モード）
	result = dInputDevice->SetCooperativeLevel(
		Application::GetInstance()->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

	//軸のモードを絶対値に設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(DIPROPDWORD);
	diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprop.diph.dwHow = DIPH_DEVICE;//対象
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;//絶対値モードに設定
	result = dInputDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	assert(SUCCEEDED(result));

	std::vector<DIDEVICEOBJECTINSTANCE> devObjectArray;
	result = dInputDevice->EnumObjects(
		DIEnumDeviceObjectCallback, 
		&devObjectArray,
		DIDFT_AXIS);//スティックのみ取得
	assert(SUCCEEDED(result));

	for (int i = 0; i < devObjectArray.size(); i++)
	{
		//軸の値の範囲を設定
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = devObjectArray[i].dwType;
		diprg.lMin = (long)-MAX_STICK_VALUE;	//最小値
		diprg.lMax = (long)MAX_STICK_VALUE;		//最大値
		dInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		//OutputDebugStringW((LPCWSTR)devObjectArray[i].tszName);//コントローラーの型の名前
	}
	ZeroMemory(&devObjectArray, sizeof(devObjectArray));
	return true;
}

void gamelib::DirectInputGamepad::Update()
{
	//ゲームパッドの入力情報取得
	memcpy(&padStatePre, &padState, sizeof(DIJOYSTATE));
	ZeroMemory(&padState, sizeof(DIJOYSTATE));
	dInputDevice->Acquire();
	dInputDevice->Poll();
	HRESULT result = dInputDevice->GetDeviceState(sizeof(DIJOYSTATE), &padState);
	if (FAILED(result))
	{
		ZeroMemory(&padState, sizeof(DIJOYSTATE));
		padState.rgdwPOV[0] = -1;
		leftThumb = rightThumb = {};
		return;
	}
	leftThumb.x = static_cast<float>(padState.lX);
	leftThumb.y = static_cast<float>(-padState.lY);
	rightThumb.x = static_cast<float>(padState.lZ);
	rightThumb.y = static_cast<float>(-padState.lRz);
	leftThumb.Normalize();
	rightThumb.Normalize();
}

bool gamelib::DirectInputGamepad::IsButton(GAMEPAD_CODE buttonCode) const
{
	int buttonNum = static_cast<int>(buttonCode);
	if (buttonNum >= DIRECTINPUT_UP)
	{
		return IsDpad(ConvertDirButton(buttonNum));
	}
	return (padState.rgbButtons[buttonNum] & 0x80);
}

bool gamelib::DirectInputGamepad::IsButtonDown(GAMEPAD_CODE buttonCode) const
{
	int buttonNum = static_cast<int>(buttonCode);
	if (buttonNum >= DIRECTINPUT_UP)
	{
		return IsDpadDown(ConvertDirButton(buttonNum));
	}
	return (padState.rgbButtons[buttonNum] & 0x80 &&
		!(padStatePre.rgbButtons[buttonNum] & 0x80));
}

bool gamelib::DirectInputGamepad::IsButtonUp(GAMEPAD_CODE buttonCode) const
{
	int buttonNum = static_cast<int>(buttonCode);
	if (buttonNum >= DIRECTINPUT_UP)
	{
		return IsDpadUp(ConvertDirButton(buttonNum));
	}
	return (!(padState.rgbButtons[buttonNum] & 0x80) &&
		padStatePre.rgbButtons[buttonNum] & 0x80);
}


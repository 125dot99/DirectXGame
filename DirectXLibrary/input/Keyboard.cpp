#include "Keyboard.h"

#include <assert.h>

#include "../app/Application.h"

void gamelib::Keyboard::CreateInputDevice()
{
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	Microsoft::WRL::ComPtr<IDirectInput8W> dinput;
	//DirectInputオブジェクトの生成
	HRESULT result = DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		nullptr);
	assert(SUCCEEDED(result));
	//キーボードを指定
	result = dinput->CreateDevice(
		GUID_SysKeyboard,
		&dInputDevice,
		nullptr);
	assert(SUCCEEDED(result));
}

gamelib::Keyboard::Keyboard()
{
	CreateInputDevice();
	//デバイスのフォーマット指定
	HRESULT result = dInputDevice->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	//画面が手前にある場合のみ
	result = dInputDevice->SetCooperativeLevel(
		Application::GetInstance()->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE /*| DISCL_NOWINKEY*/);//Windowsキーは有効にする
	assert(SUCCEEDED(result));
}

void gamelib::Keyboard::Update()
{
	memcpy(&keyPre, &key, MAX_KEY);
	ZeroMemory(key, MAX_KEY);
	dInputDevice->Acquire();
	HRESULT result = dInputDevice->GetDeviceState(sizeof(BYTE[MAX_KEY]), key);
	if (FAILED(result))//取得失敗
	{
		ZeroMemory(key, MAX_KEY);
	}
}

bool gamelib::Keyboard::IsKey(KEY_CODE keyCode) const
{
	return key[static_cast<int>(keyCode)] & 0x80;
}

bool gamelib::Keyboard::IsKeyDown(KEY_CODE keyCode) const
{
	return key[static_cast<int>(keyCode)] & 0x80 && 
		!(keyPre[static_cast<int>(keyCode)] & 0x80);
}

bool gamelib::Keyboard::IsKeyUp(KEY_CODE keyCode) const
{
	return !(key[static_cast<int>(keyCode)] & 0x80) &&
		keyPre[static_cast<int>(keyCode)] & 0x80;
}

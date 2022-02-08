#include "Mouse.h"

#include <assert.h>

#include "../app/Application.h"

gamelib::Mouse::Mouse()
{
	ShowCursor(true);//�}�E�X�J�[�\���\��
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	Microsoft::WRL::ComPtr<IDirectInput8W> dinput;
	//DirectInput�I�u�W�F�N�g�̐���
	HRESULT result = DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		nullptr);
	assert(SUCCEEDED(result));
	//�}�E�X���w��
	result = dinput->CreateDevice(
		GUID_SysMouse, 
		&dInputDevice, 
		nullptr);
	assert(SUCCEEDED(result));
	//�f�o�C�X�̃t�H�[�}�b�g�w��
	result = dInputDevice->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));
	//��ʂ���O�ɂ���ꍇ�̂�
	result = dInputDevice->SetCooperativeLevel(
		Application::GetInstance()->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));
}

void gamelib::Mouse::Update()
{
	dInputDevice->Acquire();
	memcpy(&mousePre, &mouse, sizeof(DIMOUSESTATE));
	HRESULT result = dInputDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mouse);
	if (FAILED(result))
	{
		ZeroMemory(&mouse, sizeof(DIMOUSESTATE));
		position = Vector2::Zero();
		return;
	}
	POINT pos;
	GetCursorPos(&pos);//long�^�ɃL���X�g
	ScreenToClient(Application::GetInstance()->GetHwnd(), &pos);
	position = { static_cast<float>(pos.x), static_cast<float>(pos.y) };
}

bool gamelib::Mouse::IsButton(MOUSE_CODE mouseCode) const
{
	return mouse.rgbButtons[static_cast<int>(mouseCode)] & MOUSE_VALUE;
}

bool gamelib::Mouse::IsButtonDown(MOUSE_CODE mouseCode) const
{
	return mouse.rgbButtons[static_cast<int>(mouseCode)] & MOUSE_VALUE &&
		!(mousePre.rgbButtons[static_cast<int>(mouseCode)] & MOUSE_VALUE);
}

bool gamelib::Mouse::IsButtonUp(MOUSE_CODE mouseCode) const
{
	return !(mouse.rgbButtons[static_cast<int>(mouseCode)] & MOUSE_VALUE) &&
		mousePre.rgbButtons[static_cast<int>(mouseCode)] & MOUSE_VALUE;
}

float gamelib::Mouse::GetWheel() const
{
	return static_cast<float>(mouse.lZ);
}

const gamelib::Vector2& gamelib::Mouse::GetPoint() const
{
	return position;
}

gamelib::Vector2 gamelib::Mouse::GetVelocity() const
{
	return Vector2(static_cast<float>(mouse.lX), static_cast<float>(mouse.lY));
}

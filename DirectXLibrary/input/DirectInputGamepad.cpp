#include "DirectInputGamepad.h"

#include <assert.h>
#include <vector>

#include "../app/Application.h"

//�f�o�C�X�񋓂���R�[���o�b�N�֐�
BOOL CALLBACK DIEnumDeviceCallback(LPDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	std::vector<DIDEVICEINSTANCE>* lpddis = (std::vector<DIDEVICEINSTANCE>*)pvRef;
	lpddis->emplace_back(*lpddi);//�Q�[���p�b�h�f�o�C�X���i�[
	return DIENUM_CONTINUE;	//���̃f�o�C�X���
}

//�f�o�C�X�̃I�u�W�F�N�g��񋓂���R�[���o�b�N�֐�
BOOL CALLBACK DIEnumDeviceObjectCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	std::vector<DIDEVICEOBJECTINSTANCE>* lpddois = (std::vector<DIDEVICEOBJECTINSTANCE>*)pvRef;
	lpddois->emplace_back(*lpddoi);//�Q�[���p�b�h�f�o�C�X�I�u�W�F�N�g���i�[
	return DIENUM_CONTINUE;	//���̃f�o�C�X���
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
	return MAX;//�����͈�
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

	//�f�o�C�X�̗񋓂��擾
	std::vector<DIDEVICEINSTANCE> deviceArray;
	//�W���C�p�b�h��T��
	result = dinput->EnumDevices(
		DI8DEVTYPE_JOYSTICK,//DI8DEVCLASS_GAMECTRL,
		(LPDIENUMDEVICESCALLBACK)DIEnumDeviceCallback,
		&deviceArray,
		DIEDFL_ATTACHEDONLY);
	const int INDEX = 0;
	if (deviceArray.size() <= INDEX)//DirectInput�Q�[���p�b�h�擾���s
	{
		ZeroMemory(&deviceArray, sizeof(deviceArray));
		return false;
	}
	OutputDebugStringA("DirectInput�Q�[���p�b�h��������܂���\n");
	//�������珉��������
	result = dinput->CreateDevice(
		deviceArray[INDEX].guidInstance,
		dInputDevice.ReleaseAndGetAddressOf(),
		nullptr);

	assert(SUCCEEDED(result) && "�f�o�C�X�������s");
	ZeroMemory(&deviceArray, sizeof(deviceArray));

	// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	result = dInputDevice->SetDataFormat(&c_dfDIJoystick);
	assert(SUCCEEDED(result));

	// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	result = dInputDevice->SetCooperativeLevel(
		Application::GetInstance()->GetHwnd(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

	//���̃��[�h���Βl�ɐݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(DIPROPDWORD);
	diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprop.diph.dwHow = DIPH_DEVICE;//�Ώ�
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;//��Βl���[�h�ɐݒ�
	result = dInputDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	assert(SUCCEEDED(result));

	std::vector<DIDEVICEOBJECTINSTANCE> devObjectArray;
	result = dInputDevice->EnumObjects(
		DIEnumDeviceObjectCallback, 
		&devObjectArray,
		DIDFT_AXIS);//�X�e�B�b�N�̂ݎ擾
	assert(SUCCEEDED(result));

	for (int i = 0; i < devObjectArray.size(); i++)
	{
		//���̒l�͈̔͂�ݒ�
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = devObjectArray[i].dwType;
		diprg.lMin = (long)-MAX_STICK_VALUE;	//�ŏ��l
		diprg.lMax = (long)MAX_STICK_VALUE;		//�ő�l
		dInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		//OutputDebugStringW((LPCWSTR)devObjectArray[i].tszName);//�R���g���[���[�̌^�̖��O
	}
	ZeroMemory(&devObjectArray, sizeof(devObjectArray));
	return true;
}

void gamelib::DirectInputGamepad::Update()
{
	//�Q�[���p�b�h�̓��͏��擾
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


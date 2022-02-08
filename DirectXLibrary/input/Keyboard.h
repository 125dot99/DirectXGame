#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <wrl.h>

#include "IKeyborad.h"

#pragma comment(lib, "dinput8.lib")

namespace gamelib
{
class Keyboard : public IKeyboard
{
private:
	//�L�[���͂��i�[���鐔
	static const int MAX_KEY = 256;
	//�L�[���͊i�[
	BYTE key[MAX_KEY] = {};
	//�O�t���[���̃L�[�ԍ�
	BYTE keyPre[MAX_KEY] = {};
	//�L�[�̃f�o�C�X�f�[�^
	Microsoft::WRL::ComPtr<IDirectInputDevice8W> dInputDevice;
	//���̓f�o�C�X�̐���
	void CreateInputDevice();
public:
	Keyboard();
	void Update() override;
	bool IsKey(KEY_CODE keyCode) const override;
	bool IsKeyDown(KEY_CODE keyCode) const override;
	bool IsKeyUp(KEY_CODE keyCode) const override;
};
} // namespace gamelib
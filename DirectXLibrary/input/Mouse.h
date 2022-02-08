#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <wrl.h>

#include "IMouse.h"

#pragma comment(lib,"dinput8.lib")

namespace gamelib
{
class Mouse : public IMouse
{
private:
	//�}�E�X�̃{�^������
	static const unsigned char MOUSE_VALUE = 0x80;
	//�J�[�\���ʒu�i�����^�j
	Vector2 position;
	//�}�E�X�̏��
	DIMOUSESTATE mouse{};
	//�O�t���[���̃}�E�X�̏��
	DIMOUSESTATE mousePre{};
	//�}�E�X�̃f�o�C�X
	Microsoft::WRL::ComPtr<IDirectInputDevice8W> dInputDevice;
public:
	Mouse();
	void Update() override;
	bool IsButton(MOUSE_CODE mouseCode) const override;
	bool IsButtonDown(MOUSE_CODE mouseCode) const override;
	bool IsButtonUp(MOUSE_CODE mouseCode) const override;
	float GetWheel() const override;
	const Vector2& GetPoint() const override;
	Vector2 GetVelocity() const override;
};
} // namespace gamelib


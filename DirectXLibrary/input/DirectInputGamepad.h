#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <wrl.h>
#include "IGamepad.h"

namespace gamelib
{
class DirectInputGamepad : public IGamepad
{
private:
	//DirectXInput�̃R�[�h���ʗp
	enum DIERCTINPUT_BUTTON_ENUM
	{
		DIRECTINPUT_UP = 12,
		DIRECTINPUT_DOWN,
		DIRECTINPUT_LEFT,
		DIRECTINPUT_RIGHT,
	};
	//�X�e�B�b�N���͍ő�l
	static const unsigned short MAX_STICK_VALUE = 0x8000;
	//�X�e�B�b�N�̓��͒l
	Vector2 leftThumb = {};
	Vector2 rightThumb = {};
	//DirectInput�̓��͍\����
	DIJOYSTATE padState = {};
	//�O�t���[���̓��͒l�i�[
	DIJOYSTATE padStatePre = {};
	//DirectInput�̓��͂��󂯎��f�o�C�X
	Microsoft::WRL::ComPtr<IDirectInputDevice8W> dInputDevice;
	//�\���{�^���̓���
	bool IsDpad(unsigned int value) const;
	bool IsDpadDown(unsigned int value) const;
	bool IsDpadUp(unsigned int value) const;
	unsigned int ConvertDirButton(int num) const;
public:
	bool Initialize() override;
	void Update() override;
	bool IsButton(GAMEPAD_CODE buttonCode) const override;
	bool IsButtonDown(GAMEPAD_CODE buttonCode) const override;
	bool IsButtonUp(GAMEPAD_CODE buttonCode) const override;
	inline const Vector2& GetLeftThumb() const override { return leftThumb; }
	inline const Vector2& GetRightThumb() const override { return rightThumb; }
	inline void SetLeftVibration(int power) override {}
	inline void SetRightVibration(int power) override {}
};
} // namespace gamelib
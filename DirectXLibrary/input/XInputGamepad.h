#pragma once
#include <Windows.h>
#include <Xinput.h>

#include "IGamepad.h"
#pragma comment(lib, "xinput.lib")

namespace gamelib
{
class XInputGamepad : public IGamepad
{
private:
	//XInput�̃R�[�h���ʗp
	enum XINPUT_BUTTON_ENUM
	{
		XINPUT_LTRREGER = 6,
		XINPUT_RTRREGER = 7,
	};
	//�r�b�g�ϊ��z��
	static const int MAX_BUTTON = 16;
	static unsigned int xButtons[MAX_BUTTON];
	//�X�e�B�b�N�̓��͒l
	Vector2 leftThumb = {};
	Vector2 rightThumb = {};
	//XInput�̓��͍\����
	XINPUT_STATE state = {};
	//�O�t���[���̓��͒l�i�[
	XINPUT_GAMEPAD prevGamepad = {};
	//�U�����g��
	XINPUT_VIBRATION vibration = {};
	//DirectInput�̃{�^���ƍ��킹�邽�ߕϊ�
	void SetButtonArray();
	//�g���K�[���͔���
	bool IsTriggerRangeNum(int num) const;
	bool IsTrigger(bool isLeft) const;
	bool IsTriggerDown(bool isLeft) const;
	bool IsTriggerUp(bool isLeft) const;
public:
	bool Initialize() override;
	void Update() override;
	bool IsButton(GAMEPAD_CODE buttonCode) const override;
	bool IsButtonDown(GAMEPAD_CODE buttonCode) const override;
	bool IsButtonUp(GAMEPAD_CODE buttonCode) const override;
	inline const Vector2& GetLeftThumb() const override { return leftThumb; }
	inline const Vector2& GetRightThumb() const override { return rightThumb; }
	void SetLeftVibration(int power) override;
	void SetRightVibration(int power) override;
};
} // namespace gamelib
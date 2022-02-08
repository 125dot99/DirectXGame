#pragma once
#include "IKeyborad.h"
#include "IMouse.h"
#include "IGamepad.h"
#include <memory>

namespace gamelib
{
class Input
{
private:
	static std::unique_ptr<IKeyboard> keyboard;
	static std::unique_ptr<IMouse> mouse;
	static std::unique_ptr<IGamepad> gamepad;
public:
	/// <summary>
	/// ���̓f�o�C�X����������
	/// </summary>
	static void Initialize();

	/// <summary>
	/// ���t���[������ �f�o�C�X�̓��͏����X�V
	/// </summary>
	static void Update();

	/// <summary>
	/// �L�[�{�[�h�̓���
	/// </summary>
	/// <returns>�L�[�{�[�h�̃|�C���^</returns>
	static inline IKeyboard* GetKeyborad() { return keyboard.get(); }
	
	/// <summary>
	/// �}�E�X�̓���
	/// </summary>
	/// <returns>�}�E�X�̃|�C���^</returns>
	static inline IMouse* GetMouse() { return mouse.get(); }

	/// <summary>
	/// �Q�[���p�b�h�̓���
	/// </summary>
	/// <returns>�Q�[���p�b�h�̃|�C���^</returns>
	static inline IGamepad* GetGamepad() { return gamepad.get(); }
};
} // namespace gamelib


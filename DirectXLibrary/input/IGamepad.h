#pragma once
#include "../math/Vector.h"

namespace gamelib
{
//���̓{�^����`
enum class GAMEPAD_CODE
{
	X,				//X�{�^��
	Y,				//Y�{�^��
	A,				//A�{�^��
	B,				//B�{�^��
	LEFT_SHOULDER,	//L�{�^��
	RIGHT_SHOULDER,	//R�{�^��
	LEFT_TRIGGER,	//L�g���K�[
	RIGHT_TRIGGER,	//R�g���K�[
	LEFT_THUMB,		//L�X�e�B�b�N��������
	RIGHT_THUMB,	//R�X�e�B�b�N��������
	BACK,			//Back�{�^��
	START,			//Start�{�^��
	UP,				//�\���{�^����
	DOWN,			//�\���{�^����
	LEFT,			//�\���{�^����
	RIGHT,			//�\���{�^����
};

//�Q�[���p�b�h�f�o�C�X�C���^�[�t�F�[�X
class IGamepad
{
public:
	/// <summary>
	/// ���z�f�X�g���N�^
	/// </summary>
	virtual ~IGamepad() = default;
	
	/// <summary>
	/// �Q�[���p�b�h������
	/// </summary>
	/// <returns>�f�o�C�X�𐶐��o������</returns>
	virtual bool Initialize() = 0;
	
	/// <summary>
	/// ���t���[���X�V����
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �����Ă��邩���肷��
	/// </summary>
	/// <param name="buttonNum"></param>
	/// <returns>�����Ă����ture��Ԃ�</returns>
	virtual bool IsButton(GAMEPAD_CODE buttonCode) const = 0;

	/// <summary>
	/// �������u�Ԃ̔��肷��
	/// </summary>
	/// <param name="buttonNum"></param>
	/// <returns>�������u��ture��Ԃ�</returns>
	virtual bool IsButtonDown(GAMEPAD_CODE buttonCode) const = 0;

	/// <summary>
	/// �������u�Ԃ̔��肷��
	/// </summary>
	/// <param name="buttonNum"></param>
	/// <returns>�������u��ture��Ԃ�</returns>
	virtual bool IsButtonUp(GAMEPAD_CODE buttonCode) const = 0;
	
	/// <summary>
	/// ���X�e�B�b�N�̌X�����擾����
	/// </summary>
	/// <returns>���X�e�B�b�N�̓��͒l</returns>
	virtual const Vector2& GetLeftThumb() const = 0;

	/// <summary>
	/// �E�X�e�B�b�N�̌X�����擾����
	/// </summary>
	/// <returns>�E�X�e�B�b�N�̓��͒l</returns>
	virtual const Vector2& GetRightThumb() const = 0;

	/// <summary>
	/// �R���g���[���[�����̐U���̐ݒ�
	/// </summary>
	/// <param name="power">�U�����鋭��</param>
	virtual void SetLeftVibration(int power) = 0;
	
	/// <summary>
	/// �R���g���[���[�E���̐U���̐ݒ�
	/// </summary>
	/// <param name="power">�U�����鋭��</param>
	virtual void SetRightVibration(int power) = 0;
};
} // namespace gamelib

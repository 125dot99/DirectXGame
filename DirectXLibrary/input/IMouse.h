#pragma once
#include "../math/Vector.h"

namespace gamelib
{
enum class MOUSE_CODE
{
	LEFT,
	RIGHT,
	CENTOR
};

//�}�E�X�C���^�[�t�F�[�X
class IMouse
{
public:
	/// <summary>
	/// ���z�f�X�g���N�^
	/// </summary>
	virtual ~IMouse() = default;

	/// <summary>
	/// ���t���[���X�V����
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �����Ă��邩���肷��
	/// </summary>
	/// <param name="buttonNum"></param>
	/// <returns>�����Ă����ture��Ԃ�</returns>
	virtual bool IsButton(MOUSE_CODE mouseCode) const = 0;

	/// <summary>
	/// �������u�Ԃ̔��肷��
	/// </summary>
	/// <param name="buttonNum"></param>
	/// <returns>�������u��ture��Ԃ�</returns>
	virtual bool IsButtonDown(MOUSE_CODE mouseCode) const = 0;

	/// <summary>
	/// �������u�Ԃ̔��肷��
	/// </summary>
	/// <param name="buttonNum"></param>
	/// <returns>�������u��ture��Ԃ�</returns>
	virtual bool IsButtonUp(MOUSE_CODE mouseCode) const = 0;

	/// <summary>
	/// �z�C�[���̉�]�l���擾����
	/// </summary>
	/// <returns>�z�C�[���̉�]�l</returns>
	virtual float GetWheel() const = 0;

	/// <summary>
	/// �}�E�X�J�[�\���̈ʒu���擾����
	/// </summary>
	/// <returns>�}�E�X�̈ʒu</returns>
	virtual const Vector2& GetPoint() const = 0;

	/// <summary>
	/// 1�t���[���̃}�E�X�̈ړ��ʂ��擾����
	/// </summary>
	/// <returns>�}�E�X�̈ړ���</returns>
	virtual Vector2 GetVelocity() const = 0;
};
} // namespace gamelib
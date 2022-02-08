#pragma once
namespace gamelib
{
class HitPoint final
{
private:
	float hp;
	float max;
public:
	/// <summary>
	/// �̗͂�ݒ�
	/// </summary>
	/// <param name="point"></param>
	void SetHP(const float point);

	/// <summary>
	/// �̗͂��񕜂���
	/// </summary>
	/// <param name="point">�񕜗�</param>
	void Recovery(const float point);

	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	/// <param name="point">�_���[�W��</param>
	void Damage(const float point);

	/// <summary>
	/// �̗͂��擾
	/// </summary>
	/// <param name="point"></param>
	const float& GetHP() const;

	/// <summary>
	/// �\���p�̗̑̓Q�[�W�̒l��Ԃ�
	/// </summary>
	/// <param name="spriteSize"></param>
	/// <returns></returns>
	float GetLifeGage(const float spriteSize) const;

	/// <summary>
	/// �̗͎͂c���Ă��邩
	/// </summary>
	/// <returns></returns>
	bool IsDead() const;
};
} // namespace gamelib


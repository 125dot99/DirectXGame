#pragma once
namespace gamelib
{
/// <summary>
/// ���b�V���C���^�[�t�F�[�X
/// </summary>
struct IMesh abstract
{
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~IMesh() = default;

	/// <summary>
	/// �o�b�t�@����
	/// </summary>
	virtual void CreateBuffers() = 0;

	/// <summary>
	/// �`��R�}���h
	/// </summary>
	virtual void Draw() = 0;
};
} // namespace gamelib

#pragma once
namespace gamelib
{
/// <summary>
/// �}�e���A���C���^�[�t�F�[�X
/// </summary>
struct IMaterial abstract
{
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~IMaterial() = default;

	/// <summary>
	/// �o�b�t�@����
	/// </summary>
	virtual void Create() = 0;

	/// <summary>
	/// �}�e���A����GPU�R�}���h
	/// </summary>
	virtual void Register() = 0;
};
} // namespace gamelib

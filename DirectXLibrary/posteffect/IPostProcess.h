#pragma once
namespace gamelib
{
class IPostProcess
{
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~IPostProcess() = default;

	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	/// <param name="pPost"></param>
	virtual void Befoer() = 0;

	/// <summary>
	/// �V�[���`��㏈��
	/// </summary>
	/// <param name="pPost"></param>
	virtual void After() = 0;
};
} // namespace gamelib
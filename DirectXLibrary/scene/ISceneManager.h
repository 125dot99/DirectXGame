#pragma once
namespace gamelib
{
class ISceneManager
{
public:
	virtual ~ISceneManager() = default;

	/// <summary>
	/// �|�C���^�t���ւ�
	/// </summary>
	/// <param name="newScene">���̃V�[���̃|�C���^</param>
	virtual void ChangeScene(class BaseScene* newScene) = 0;
};
} // namespace gamelib
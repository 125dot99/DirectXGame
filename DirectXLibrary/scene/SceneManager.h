#pragma once
#include "BaseScene.h"
#include <memory>
namespace gamelib
{
//�V�[���Ǘ��N���X
class SceneManager : public ISceneManager
{
private:
	/// <summary>
	/// �V�[���̃|�C���^
	/// </summary>
	std::unique_ptr<BaseScene> scene;

	/// <summary>
	/// ���̃V�[��
	/// </summary>
	void Next();

	/// <summary>
	/// ���̃V�[����������
	/// </summary>
	void ResetScene();
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SceneManager(BaseScene* pMainRootScene);
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SceneManager();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �|�C���^�t���ւ�
	/// </summary>
	/// <param name="newScene">���̃V�[���̃|�C���^</param>
	void ChangeScene(BaseScene* newScene) override;
};
} // namespace scene
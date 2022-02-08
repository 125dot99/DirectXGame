#pragma once
#include "ISceneManager.h"
namespace gamelib
{
//�V�[�����ۃN���X
class BaseScene abstract
{
protected:
	/// <summary>
	/// �V�[����؂�ւ��邩
	/// </summary>
	bool nextSceneState;
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~BaseScene() = default;

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Initialize() = 0;
	
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() = 0;
	
	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() = 0;
	
	/// <summary>
	/// �I������
	/// </summary>
	virtual void Finalize() = 0;
	
	/// <summary>
	/// �V�[���؂�ւ�����
	/// </summary>
	/// <param name="sceneManager">�V�[���}�l�[�W���[�̃|�C���^</param>
	virtual void NextScene(ISceneManager* pSceneManager) = 0;
	
	/// <summary>
	/// �V�[����؂�ւ����Ԃ�Ԃ�
	/// </summary>
	/// <returns>�V�[����؂�ւ��邩</returns>
	inline bool GetNextScene() const { return nextSceneState; }
};

//�V�[���̎q�N���X
class BaseChildScene
{
public:
	BaseChildScene() = default;
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~BaseChildScene() = default;

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() {}

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() {}

	/// <summary>
	/// �V�[���̏�Ԃ�Ԃ�
	/// </summary>
	virtual bool IsState(const char* typeName) const { return "None" == typeName; }

	/// <summary>
	/// ���̃V�[���ɐ؂�ւ�邩
	/// </summary>
	virtual bool IsNext() const { return false; }
};
} // namespace scene
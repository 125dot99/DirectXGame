#pragma once
#include <memory>
#include "../dx12/IMesh.h"
#include "../dx12/ConstBuffer.h" 
#include "../math/Matrix4.h"
#include "../loader/FbxLoader.h"

namespace gamelib
{
struct FbxAnimaParameter;

struct CBMatrixBones //�{�[���p
{
	static const int MAX_BONES = 32;
	Matrix4 bones[MAX_BONES];
	void Copy()
	{
		for (int i = 0; i < MAX_BONES; i++)
		{
			bones[i] = MatrixIdentity();
		}
	}
};

class FbxAnimation
{
private:
	int index;
	int animaStack;
	int boneSize;
	//�{�[���]���p�o�b�t�@
	std::unique_ptr<ConstBuffer> cbuffer;
	//�A�j���[�V�����f�[�^
	std::unique_ptr<FbxAnimaParameter> animaParam;
	//���f���̃{�[��
	FbxBone* pMeshBone;
	//FBX�V�[��
	fbxsdk::FbxScene* fbxScene;

public:
	FbxAnimation(IMesh* p_mesh);
	~FbxAnimation();

	void Play(int index);

	/// <summary>
	/// �A�j���[�V�����̍X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �{�[���]���p
	/// </summary>
	void GraphicsCommand();
};
} // namespace gamelib

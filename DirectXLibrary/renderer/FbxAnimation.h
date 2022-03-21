#pragma once
#include <memory>
#include "../dx12/IMesh.h"
#include "../dx12/ConstBuffer.h" 
#include "../math/Matrix4.h"
#include "../loader/FbxLoader.h"

namespace gamelib
{
struct FbxAnimaParameter;

struct CBMatrixBones //ボーン用
{
	static const int MAX_BONES = 32;
	Matrix4 vecFbxBones[MAX_BONES];
	void Copy()
	{
		for (int i = 0; i < MAX_BONES; i++)
		{
			vecFbxBones[i] = MatrixIdentity();
		}
	}
};

class FbxAnimation
{
private:
	int index;
	int animaStack;
	int boneSize;
	//ボーン転送用バッファ
	std::unique_ptr<ConstBuffer> u_pConstBuffer;
	//アニメーションデータ
	std::unique_ptr<FbxAnimaParameter> u_pAnimaParam;
	//モデルのボーン
	FbxBone* pMeshBone;
	//FBXシーン
	fbxsdk::FbxScene* pFbxScene;

public:
	FbxAnimation(const std::string& skinmeshName);
	~FbxAnimation();

	void Play(int index);

	/// <summary>
	/// アニメーションの更新
	/// </summary>
	void Update();

	/// <summary>
	/// ボーン転送用
	/// </summary>
	void GraphicsCommand();
};
} // namespace gamelib

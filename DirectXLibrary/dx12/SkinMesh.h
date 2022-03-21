#pragma once
#include <string>

#include "IMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "../math/Matrix4.h"

//fbxsdk��p
namespace fbxsdk
{
class FbxScene;
class FbxCluster;
} // namespace fbxsdk

namespace gamelib
{
//�m�[�h
struct FbxMeshNode
{
	std::string name;
	Matrix4 transform;			//���[�J���ό`�s��
	Matrix4 globalTransform;	//�O���[�o���ό`�s��
	FbxMeshNode* pParentNode;	//�e�m�[�h
	FbxMeshNode()
	{
		transform = MatrixIdentity();
		globalTransform = MatrixIdentity();
		pParentNode = nullptr;
	}
};

//�{�[���\����
struct FbxBone
{
	std::string name;
	Matrix4 invInitialPose;	//�����p���̋t�s��
	fbxsdk::FbxCluster* fbxCluster;	//�N���X�^�[
	FbxBone(const std::string& _name) : name(_name), fbxCluster(nullptr) {}
};

struct SkinMesh : IMesh
{
	std::string name;	
	std::unique_ptr<VertexBuffer<VertexNormalUvBones>> u_pVertexBuffer;
	std::unique_ptr<IndexBuffer> u_pIndexBuffer;

	//�m�[�h�z��
	std::vector<FbxMeshNode> vecFbxMeshNodes;
	//�{�[���z��
	std::vector<FbxBone> vecFbxBones;
	//FBX�V�[��
	fbxsdk::FbxScene* pFbxScene;

	SkinMesh();

	void CreateBuffers() override;
	void Draw() override;
};
} // namespace gamelib

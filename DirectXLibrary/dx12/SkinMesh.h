#pragma once
#include "IMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
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
	FbxMeshNode* parent;		//�e�m�[�h
	FbxMeshNode()
	{
		transform = MatrixIdentity();
		globalTransform = MatrixIdentity();
		parent = nullptr;
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
	std::unique_ptr<VertexBuffer<VertexNormalUvBones>> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::unique_ptr<Material> material;

	//�m�[�h�z��
	std::vector<FbxMeshNode> nodes;
	//�{�[���z��
	std::vector<FbxBone> bones;
	//FBX�V�[��
	fbxsdk::FbxScene* fbxScene;

	SkinMesh();
	~SkinMesh();

	void CreateBuffers() override;
	void Draw(bool isMtlCommand = true) override;
};
} // namespace gamelib

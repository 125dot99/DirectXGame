#pragma once
#include "../dx12/SkinMesh.h"
#include <string>

namespace fbxsdk
{
class FbxNode;
class FbxMesh;
}

namespace gamelib
{
class FbxLoader
{
private:
	FbxLoader() = delete;
	~FbxLoader() = delete;
	static SkinMesh* LoadModelFromFile(const std::string& modelName);
	//�ċA�I�Ƀm�[�h�\�������
	static void ParseNodeRecursive(SkinMesh* model, fbxsdk::FbxNode* fbxNode, FbxMeshNode* parent = nullptr);
	//���b�V���ǂݍ���
	static void ParseMesh(SkinMesh* model, fbxsdk::FbxNode* fbxNode);
	//���_���W�ǂݎ��
	static void ParseMeshPoints(SkinMesh* model, fbxsdk::FbxMesh* fbxMesh);
	//���_�C���f�b�N�X�ǂݎ��
	static void ParseMeshIndices(SkinMesh* model, fbxsdk::FbxMesh* fbxMesh);
	//���_uv���W�ǂݎ��
	static void ParseMeshUVs(std::vector<Vector2>& uvs, fbxsdk::FbxMesh* fbxMesh);
	//���_�@���ǂݎ��
	static void ParseMeshNormals(std::vector<Vector3>& normals, fbxsdk::FbxMesh* fbxMesh);
	//���_�f�[�^����蒼��
	static void RemakeMeshVertices(SkinMesh* model, fbxsdk::FbxMesh* fbxMesh);
	//�}�e���A���ǂݍ���
	static void ParseMaterial(SkinMesh* model, fbxsdk::FbxNode* fbxNode);

	/// <summary>
	/// �X�L�j���O���̓ǂݎ��
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxMesh"></param>
	static void ParseSkin(SkinMesh* model, fbxsdk::FbxMesh* fbxMesh);
public:
	/// <summary>
	/// FBX�C���|�[�^�[�AFBX�}�l�[�W���[�̐���
	/// </summary>
	static void Initialize();

	/// <summary>
	/// FBX�V�[���AFBX�C���|�[�^�[�AFBX�}�l�[�W���[�̃��������
	/// </summary>
	static void Finalize();

	/// <summary>
	/// �ǂݎ��
	/// </summary>
	/// <param name="modelName"></param>
	/// <returns></returns>
	static SkinMesh* ReadFbxModel(const std::string& modelName);

	static void* ReadFbxScene(const std::string& modelName);
};
} // namespace gamelib


#pragma once
#include "IMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "../math/Matrix4.h"

//fbxsdk専用
namespace fbxsdk
{
class FbxScene;
class FbxCluster;
} // namespace fbxsdk

namespace gamelib
{
//ノード
struct FbxMeshNode
{
	std::string name;
	Matrix4 transform;			//ローカル変形行列
	Matrix4 globalTransform;	//グローバル変形行列
	FbxMeshNode* parent;		//親ノード
	FbxMeshNode()
	{
		transform = MatrixIdentity();
		globalTransform = MatrixIdentity();
		parent = nullptr;
	}
};

//ボーン構造体
struct FbxBone
{
	std::string name;
	Matrix4 invInitialPose;	//初期姿勢の逆行列
	fbxsdk::FbxCluster* fbxCluster;	//クラスター
	FbxBone(const std::string& _name) : name(_name), fbxCluster(nullptr) {}
};

struct SkinMesh : IMesh
{
	std::string name;	
	std::unique_ptr<VertexBuffer<VertexNormalUvBones>> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::unique_ptr<Material> material;

	//ノード配列
	std::vector<FbxMeshNode> nodes;
	//ボーン配列
	std::vector<FbxBone> bones;
	//FBXシーン
	fbxsdk::FbxScene* fbxScene;

	SkinMesh();
	~SkinMesh();

	void CreateBuffers() override;
	void Draw(bool isMtlCommand = true) override;
};
} // namespace gamelib

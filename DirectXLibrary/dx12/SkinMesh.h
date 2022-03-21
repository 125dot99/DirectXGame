#pragma once
#include <string>

#include "IMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

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
	FbxMeshNode* pParentNode;	//親ノード
	FbxMeshNode()
	{
		transform = MatrixIdentity();
		globalTransform = MatrixIdentity();
		pParentNode = nullptr;
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
	std::unique_ptr<VertexBuffer<VertexNormalUvBones>> u_pVertexBuffer;
	std::unique_ptr<IndexBuffer> u_pIndexBuffer;

	//ノード配列
	std::vector<FbxMeshNode> vecFbxMeshNodes;
	//ボーン配列
	std::vector<FbxBone> vecFbxBones;
	//FBXシーン
	fbxsdk::FbxScene* pFbxScene;

	SkinMesh();

	void CreateBuffers() override;
	void Draw() override;
};
} // namespace gamelib

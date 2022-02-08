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
	//再帰的にノード構成を解析
	static void ParseNodeRecursive(SkinMesh* model, fbxsdk::FbxNode* fbxNode, FbxMeshNode* parent = nullptr);
	//メッシュ読み込み
	static void ParseMesh(SkinMesh* model, fbxsdk::FbxNode* fbxNode);
	//頂点座標読み取り
	static void ParseMeshPoints(SkinMesh* model, fbxsdk::FbxMesh* fbxMesh);
	//頂点インデックス読み取り
	static void ParseMeshIndices(SkinMesh* model, fbxsdk::FbxMesh* fbxMesh);
	//頂点uv座標読み取り
	static void ParseMeshUVs(std::vector<Vector2>& uvs, fbxsdk::FbxMesh* fbxMesh);
	//頂点法線読み取り
	static void ParseMeshNormals(std::vector<Vector3>& normals, fbxsdk::FbxMesh* fbxMesh);
	//頂点データを作り直す
	static void RemakeMeshVertices(SkinMesh* model, fbxsdk::FbxMesh* fbxMesh);
	//マテリアル読み込み
	static void ParseMaterial(SkinMesh* model, fbxsdk::FbxNode* fbxNode);

	/// <summary>
	/// スキニング情報の読み取り
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxMesh"></param>
	static void ParseSkin(SkinMesh* model, fbxsdk::FbxMesh* fbxMesh);
public:
	/// <summary>
	/// FBXインポーター、FBXマネージャーの生成
	/// </summary>
	static void Initialize();

	/// <summary>
	/// FBXシーン、FBXインポーター、FBXマネージャーのメモリ解放
	/// </summary>
	static void Finalize();

	/// <summary>
	/// 読み取り
	/// </summary>
	/// <param name="modelName"></param>
	/// <returns></returns>
	static SkinMesh* ReadFbxModel(const std::string& modelName);

	static void* ReadFbxScene(const std::string& modelName);
};
} // namespace gamelib


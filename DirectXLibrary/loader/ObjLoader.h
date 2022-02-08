#pragma once
#include "../dx12/Mesh.h"
#include <string>

namespace gamelib
{
class ObjLoader
{
private:
	ObjLoader() = delete;
	~ObjLoader() = delete;

	/// <summary>
	/// objファイルのメッシュ読み込み
	/// </summary>
	/// <param name="pModel"></param>
	/// <returns></returns>
	static Mesh* LoadModelFromFile(const std::string& modelName);

	/// <summary>
	/// objファイルのマテリアル読み込み
	/// </summary>
	/// <param name="pModel"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static void LoadMaterialFile(Mesh* model, const std::string& fileName);
public:
	/// <summary>
	/// 読み取り
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static Mesh* ReadObjModel(const std::string& modelName);
};
} // namespace gamelib

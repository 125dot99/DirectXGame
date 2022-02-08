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
	/// obj�t�@�C���̃��b�V���ǂݍ���
	/// </summary>
	/// <param name="pModel"></param>
	/// <returns></returns>
	static Mesh* LoadModelFromFile(const std::string& modelName);

	/// <summary>
	/// obj�t�@�C���̃}�e���A���ǂݍ���
	/// </summary>
	/// <param name="pModel"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static void LoadMaterialFile(Mesh* model, const std::string& fileName);
public:
	/// <summary>
	/// �ǂݎ��
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static Mesh* ReadObjModel(const std::string& modelName);
};
} // namespace gamelib

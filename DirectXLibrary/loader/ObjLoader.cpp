#include "ObjLoader.h"

#include <sstream>
#include <fstream>
#include <assert.h>
#include <unordered_map>

#include "ResourceManager.h"
#include "../dx12/Material.h"

const std::string OBJ_PATH = "Resources/Model/Obj/";

gamelib::Mesh* gamelib::ObjLoader::LoadModelFromFile(const std::string& modelName)
{
	const std::string directoryPath = OBJ_PATH + modelName + "/";
	const std::string fileName = modelName + ".obj";
	const std::string fullpath = directoryPath + fileName;
	std::ifstream inFile;
	inFile.open(fullpath);
	
	//ファイルチェック
	if (inFile.fail())
	{
		return nullptr;
	}

	Mesh* model = new Mesh;
	model->name = modelName;
	
	//頂点情報読み取り用データ
	std::vector<Vector3> v;		//頂点座標
	std::vector<Vector2> vt;	//テクスチャ座標
	std::vector<Vector3> vn;	//法線
	std::vector<bool> smooth;	//スムースシェーディングするか
	std::string line;
	unsigned short vertexIndex = 0;
	unsigned short index = 0;
	
	//頂点データ格納用
	std::vector<VertexNormalUv>& vertices = model->u_pVertexBuffer->vertices;
	std::vector<unsigned short>& indices = model->u_pIndexBuffer->indices;
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
	while (std::getline(inFile, line))
	{
		std::istringstream line_stream(line);
		std::string key;
		//空白まで読み取る
		std::getline(line_stream, key, ' ');
		if (key == "mtllib")//マテリアル読み込み
		{
			std::getline(line_stream, key, ' ');
			LoadMaterialFile(model, OBJ_PATH + modelName + "/" + key);
		}
		else if (key == "o")//グループ
		{

		}
		else if (key == "v")//頂点座標
		{
			Vector3 position;
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			v.emplace_back(position);
		}
		else if (key == "vt")//テクスチャ座標
		{
			Vector2 uv;
			line_stream >> uv.x;
			line_stream >> uv.y;
			uv.y = 1.0f - uv.y;
			vt.emplace_back(uv);
		}
		else if (key == "vn")//法線
		{
			Vector3 normal;
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			vn.emplace_back(normal);
		}
		else if (key == "s")//スムージング用
		{
			std::string str;
			line_stream >> str;
			smooth.emplace_back(str == "1");
		}
		else if (key == "f")
		{
			std::string index_string;
			unsigned short vcount = 0;
			//スペースが見つかれば次
			while (std::getline(line_stream, index_string, ' '))
			{
				std::istringstream index_stream(index_string);
				VertexNormalUv vertex;
				unsigned short indexPosition, indexNormal, indexUv;
				index_stream >> indexPosition;
				index_stream.ignore();
				index_stream >> indexUv;
				index_stream.ignore();
				index_stream >> indexNormal;
				vertex = { v[indexPosition - 1], vn[indexNormal - 1], vt[indexUv - 1] };
				vertices.emplace_back(vertex);
				smoothData[indexPosition].emplace_back(vertexIndex++);
				//4頂点分
				if (vcount++ == 3)
				{
					//四角形の0, 1, 2, 3の内 2, 3, 0で三角形を構築する
					indices.emplace_back(index - 1);
					indices.emplace_back(index);
					indices.emplace_back(index - 3);
				}
				else
				{
					indices.emplace_back(index);
				}
				index++;
			}
		}
	}
	inFile.close();
	vertices.shrink_to_fit();
	indices.shrink_to_fit();

	//スムージング計算
	int smoothDataSize = (int)smooth.size();
	for (int i = 0; i < smoothDataSize; i++)
	{
		if (!smooth[i])
		{
			break;
		}
		for (auto it = smoothData.begin(), end = smoothData.end(); it != end; ++it)
		{
			std::vector<unsigned short>& v = it->second;
			Vector3 normal;
			for (unsigned short index : v)
			{
				normal += vertices[index].normal;
			}
			normal = normal.Normalize();
			for (unsigned short index : v)
			{
				vertices[index].normal = normal;
			}
		}
	}
	return model;
}

void gamelib::ObjLoader::LoadMaterialFile(Mesh* pModel, const std::string& fileName)
{
	//ファイルオープン
	std::ifstream inFile(fileName);
	//ファイルチェック
	assert(inFile.is_open() && "mtlファイルが見つかりません");
	//マテリアルのパス
	const std::string mtlPath = OBJ_PATH + pModel->name + "/";
	//参照
	PhongMaterial* material = new PhongMaterial;
	std::string line;
	auto resourceManager = ResourceManager::GetInstance();
	while (std::getline(inFile, line))
	{
		std::istringstream line_stream(line);
		std::string key;
		std::getline(line_stream, key, ' ');
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}
		else if (key == "newmtl")
		{
			line_stream >> material->name;
		}
		else if (key == "Ka")//アンビエントカラー
		{
			line_stream >> material->ambient.x;
			line_stream >> material->ambient.y;
			line_stream >> material->ambient.z;
		}
		else if (key == "Kd")//ディフューズカラー
		{
			line_stream >> material->diffuse.x;
			line_stream >> material->diffuse.y;
			line_stream >> material->diffuse.z;
		}
		else if (key == "Ks")//スペキュラーカラー
		{
			line_stream >> material->specular.x;
			line_stream >> material->specular.y;
			line_stream >> material->specular.z;
		}
		else if (key == "d")//アルファ値
		{
			line_stream >> material->alpha;
		}
		else if (key == "map_Kd")//メインテクスチャ
		{
			std::string kd;
			line_stream >> kd;
			resourceManager->LoadTextureFromFile(mtlPath + kd);
			material->w_pTexture = (resourceManager->GetTexture(kd));
		}
		//else if (key == "map_Bump")//ノーマルマップ
		//{
		//	std::string bump;
		//	line_stream >> bump;
		//	resourceManager->LoadTextureFromFile(mtlPath + bump);
		//	material->vec_w_pTextures.emplace_back(resourceManager->GetTexture(bump));
		//}
	}
	inFile.close();
	material->name = pModel->name;
	if (!material->w_pTexture.lock())
	{
		//1x1の白いテクスチャを適応
		material->w_pTexture = resourceManager->GetDefalutTexture();
	}
	resourceManager->GetInstance()->AddMaterial(material, material->name);
}

gamelib::Mesh* gamelib::ObjLoader::ReadObjModel(const std::string& modelName)
{
	return LoadModelFromFile(modelName);
}

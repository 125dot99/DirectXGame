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
	
	//�t�@�C���`�F�b�N
	if (inFile.fail())
	{
		return nullptr;
	}

	Mesh* model = new Mesh;
	model->name = modelName;
	
	//���_���ǂݎ��p�f�[�^
	std::vector<Vector3> v;		//���_���W
	std::vector<Vector2> vt;	//�e�N�X�`�����W
	std::vector<Vector3> vn;	//�@��
	std::vector<bool> smooth;	//�X���[�X�V�F�[�f�B���O���邩
	std::string line;
	unsigned short vertexIndex = 0;
	unsigned short index = 0;
	
	//���_�f�[�^�i�[�p
	std::vector<VertexNormalUv>& vertices = model->u_pVertexBuffer->vertices;
	std::vector<unsigned short>& indices = model->u_pIndexBuffer->indices;
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
	while (std::getline(inFile, line))
	{
		std::istringstream line_stream(line);
		std::string key;
		//�󔒂܂œǂݎ��
		std::getline(line_stream, key, ' ');
		if (key == "mtllib")//�}�e���A���ǂݍ���
		{
			std::getline(line_stream, key, ' ');
			LoadMaterialFile(model, OBJ_PATH + modelName + "/" + key);
		}
		else if (key == "o")//�O���[�v
		{

		}
		else if (key == "v")//���_���W
		{
			Vector3 position;
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			v.emplace_back(position);
		}
		else if (key == "vt")//�e�N�X�`�����W
		{
			Vector2 uv;
			line_stream >> uv.x;
			line_stream >> uv.y;
			uv.y = 1.0f - uv.y;
			vt.emplace_back(uv);
		}
		else if (key == "vn")//�@��
		{
			Vector3 normal;
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			vn.emplace_back(normal);
		}
		else if (key == "s")//�X���[�W���O�p
		{
			std::string str;
			line_stream >> str;
			smooth.emplace_back(str == "1");
		}
		else if (key == "f")
		{
			std::string index_string;
			unsigned short vcount = 0;
			//�X�y�[�X��������Ύ�
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
				//4���_��
				if (vcount++ == 3)
				{
					//�l�p�`��0, 1, 2, 3�̓� 2, 3, 0�ŎO�p�`���\�z����
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

	//�X���[�W���O�v�Z
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
	//�t�@�C���I�[�v��
	std::ifstream inFile(fileName);
	//�t�@�C���`�F�b�N
	assert(inFile.is_open() && "mtl�t�@�C����������܂���");
	//�}�e���A���̃p�X
	const std::string mtlPath = OBJ_PATH + pModel->name + "/";
	//�Q��
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
		else if (key == "Ka")//�A���r�G���g�J���[
		{
			line_stream >> material->ambient.x;
			line_stream >> material->ambient.y;
			line_stream >> material->ambient.z;
		}
		else if (key == "Kd")//�f�B�t���[�Y�J���[
		{
			line_stream >> material->diffuse.x;
			line_stream >> material->diffuse.y;
			line_stream >> material->diffuse.z;
		}
		else if (key == "Ks")//�X�y�L�����[�J���[
		{
			line_stream >> material->specular.x;
			line_stream >> material->specular.y;
			line_stream >> material->specular.z;
		}
		else if (key == "d")//�A���t�@�l
		{
			line_stream >> material->alpha;
		}
		else if (key == "map_Kd")//���C���e�N�X�`��
		{
			std::string kd;
			line_stream >> kd;
			resourceManager->LoadTextureFromFile(mtlPath + kd);
			material->w_pTexture = (resourceManager->GetTexture(kd));
		}
		//else if (key == "map_Bump")//�m�[�}���}�b�v
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
		//1x1�̔����e�N�X�`����K��
		material->w_pTexture = resourceManager->GetDefalutTexture();
	}
	resourceManager->GetInstance()->AddMaterial(material, material->name);
}

gamelib::Mesh* gamelib::ObjLoader::ReadObjModel(const std::string& modelName)
{
	return LoadModelFromFile(modelName);
}

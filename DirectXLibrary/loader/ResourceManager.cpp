#include "ResourceManager.h"

#include <assert.h>

#include "../dx12/DescriptorHeap.h"
#include "UtilityString.h"
#include "ResourceTextureHelper.h"

#include "ObjLoader.h"
#include "FbxLoader.h"
#include "WavLoader.h"

gamelib::ResourceManager::ResourceManager()
{
	FbxLoader::Initialize();
	Sound::XAudio2Initialize();
	const int MAX_RESOURCE_TEXTURE = 512;
	descriptorHeapSRV = std::make_shared<DescriptorHeap>();
	descriptorHeapSRV->Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, MAX_RESOURCE_TEXTURE);
	LoadTextureFromFile("Resources/Texture/white.png");
}

gamelib::ResourceManager::~ResourceManager()
{
	FbxLoader::Finalize();
	Sound::XAudio2Finalize();
}

gamelib::ResourceManager* gamelib::ResourceManager::GetInstance()
{
	static ResourceManager instance;
	return &instance;
}

bool gamelib::ResourceManager::LoadModelFromFile(const std::string& _fileName)
{
	if (u_map_meshs.find(_fileName) != u_map_meshs.end())
	{
		std::string str = "'" + _fileName + "'" + " : 指定したファイルは既に読み込んでいます\n";
		OutputDebugStringA(str.c_str());
		return false;
	}
	auto newObjModel = ObjLoader::ReadObjModel(_fileName);
	if (newObjModel)
	{
		u_map_meshs[_fileName].reset(newObjModel);
		u_map_meshs[_fileName]->CreateBuffers();
		return true;
	}
	auto newFbxModel = FbxLoader::ReadFbxModel(_fileName);
	if (newFbxModel)
	{
		u_map_meshs[_fileName].reset(newFbxModel);
		u_map_meshs[_fileName]->CreateBuffers();
		return true;
	}
	return false;
}

bool gamelib::ResourceManager::LoadTextureFromFile(const std::string& _fileName)
{
	std::string name = utility_string::ExtractFileName(_fileName);
	if (u_map_textures.find(name) != u_map_textures.end())
	{
		return false;
	}
	u_map_textures[name] = std::make_shared<Texture>(ResourceTextureHelper::ReadTextureBuffer(_fileName), name);
	u_map_textures[name]->CreateSRV(descriptorHeapSRV, (UINT)u_map_textures.size() - 1);
	return true;
}

bool gamelib::ResourceManager::LoadSoundFromFile(const std::string& _fileName)
{
	auto sound = WavLoader::ReadWaveFile(_fileName);
	if (sound)
	{
		u_map_wavSounds[_fileName].reset(sound);
	}
	return true;
}

std::weak_ptr<gamelib::IMesh> gamelib::ResourceManager::GetModel(const std::string& _fileName) const
{
	if (u_map_meshs.find(_fileName) != u_map_meshs.end())
	{
		return u_map_meshs.at(_fileName);
	}
	assert(!"指定したメッシュファイルは読み込まれていません");
	return std::weak_ptr<IMesh>();
}

std::weak_ptr<gamelib::Texture> gamelib::ResourceManager::GetDefalutTexture() const
{
	return u_map_textures.at("white.png");
}

std::weak_ptr<gamelib::Texture> gamelib::ResourceManager::GetTexture(const std::string& _fileName) const
{
	if (u_map_textures.find(_fileName) != u_map_textures.end())
	{
		return u_map_textures.at(_fileName);
	}
	assert(!"指定したテクスチャファイルは読み込まれていません");
	return std::weak_ptr<Texture>();
}

std::weak_ptr<gamelib::WavSound> gamelib::ResourceManager::GetSound(const std::string& _fileName) const
{
	if (u_map_wavSounds.find(_fileName) != u_map_wavSounds.end())
	{
		return u_map_wavSounds.at(_fileName);
	}
	assert(!"指定したサウンドファイルは読み込まれていません");
	return std::weak_ptr<WavSound>();
}

void gamelib::ResourceManager::RemakeMaterial(const std::string& _modelName, const Material& material)
{
	auto mtl = dynamic_cast<SkinMesh*>(GetModel(_modelName).lock().get())->material.get();
	mtl->ambient = material.ambient;
	mtl->diffuse = material.diffuse;
	mtl->specular = material.specular;
	mtl->alpha = material.alpha;
	mtl->Initialize();
}

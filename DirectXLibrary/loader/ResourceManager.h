#pragma once
#include <memory>
#include <unordered_map>
#include "../dx12/Texture.h"
#include "../dx12/IMaterial.h"
#include "../dx12/IMesh.h"
#include "../sound/Sound.h"

namespace gamelib
{
class ResourceManager
{
private:
	std::shared_ptr<class DescriptorHeap> s_pDescriptorHeapSRV;
	std::unordered_map<std::string, std::shared_ptr<Texture>> u_map_textures;
	std::unordered_map<std::string, std::shared_ptr<IMesh>> u_map_meshs;
	std::unordered_map<std::string, std::shared_ptr<IMaterial>> u_map_materials;
	std::unordered_map<std::string, std::shared_ptr<WavSound>> u_map_wavSounds;

	ResourceManager();
	~ResourceManager();
	ResourceManager(const ResourceManager&) = delete;
	void operator=(const ResourceManager&) = delete;
public:
	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static ResourceManager* GetInstance();

	/// <summary>
	/// フォルダ名を指定して読み込む
	/// </summary>
	/// <param name="modelName"></param>
	bool LoadModelFromFile(const std::string& _fileName);

	/// <summary>
	/// テクスチャの読み込み
	/// </summary>
	/// <param name="_textureName"></param>
	bool LoadTextureFromFile(const std::string& _fileName);

	/// <summary>
	/// サウンドデータの読み込み
	/// </summary>
	/// <param name="_textureName"></param>
	bool LoadSoundFromFile(const std::string& _fileName);

	/// <summary>
	/// マテリアルの追加
	/// </summary>
	/// <param name="newMaterial"></param>
	/// <param name="_fileName"></param>
	/// <returns></returns>
	bool AddMaterial(IMaterial* newMaterial, const std::string& _fileName);

	/// <summary>
	/// ファイル名を指定してメッシュを取得
	/// </summary>
	/// <param name="modelName"></param>
	/// <returns></returns>
	std::weak_ptr<IMesh> GetMesh(const std::string& _fileName) const;
	
	/// <summary>
	/// ファイル名を指定してマテリアルを取得
	/// </summary>
	/// <param name="_fileName"></param>
	/// <returns></returns>
	std::weak_ptr<IMaterial> GetMaterial(const std::string& _fileName) const;

	std::weak_ptr<Texture> GetDefalutTexture() const;

	/// <summary>
	/// ファイル名を指定してテクスチャを取得
	/// </summary>
	/// <param name="_fileName"></param>
	/// <returns></returns>
	std::weak_ptr<Texture> GetTexture(const std::string& _fileName) const;

	/// <summary>
	/// ファイル名を指定してサウンドデータを取得
	/// </summary>
	/// <param name="modelName"></param>
	/// <returns></returns>
	std::weak_ptr<WavSound> GetSound(const std::string& _fileName) const;
};
} // namespace gamelib
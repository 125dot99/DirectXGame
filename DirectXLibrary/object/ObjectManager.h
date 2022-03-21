#pragma once
#include <unordered_map>
#include "GameObject.h"

namespace gamelib
{
class ObjectManager
{
private:
	std::unordered_map<std::string, std::vector<std::unique_ptr<GameObject>>> u_mapGameObjects;
public:
	ObjectManager() = default;
	~ObjectManager() = default;

	/// <summary>
	/// オブジェクトを追加
	/// </summary>
	/// <param name="p_gameObject"></param>
	/// <param name="groupIndex"></param>
	void Add(const std::string& objectName, GameObject* pGameObject, bool isActive = true);
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 全て更新
	/// </summary>
	void Update();

	/// <summary>
	/// 名前を指定してオブジェクトを取得
	/// </summary>
	/// <param name="objectName"></param>
	/// <returns></returns>
	GameObject* Find(const std::string& objectName) const;
	
	/// <summary>
	/// 名前とインデックスを指定してオブジェクトを取得
	/// </summary>
	/// <param name="objectName"></param>
	/// <param name="index"></param>
	/// <returns></returns>
	GameObject* Find(const std::string& objectName, UINT index) const;
};
} // namespace gamelib


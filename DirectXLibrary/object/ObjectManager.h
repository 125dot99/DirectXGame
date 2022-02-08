#pragma once
#include <unordered_map>
#include "GameObject.h"

namespace gamelib
{
class ObjectManager
{
private:
	std::unordered_map<std::string, std::vector<GameObject*>> u_map_gameObjects;
public:
	ObjectManager();
	~ObjectManager();

	/// <summary>
	/// オブジェクトを追加
	/// </summary>
	/// <param name="p_gameObject"></param>
	/// <param name="groupIndex"></param>
	void Add(GameObject* p_gameObject, bool isActive = true);
	
	/// <summary>
	/// オブジェクトを削除
	/// </summary>
	void Remove(GameObject* p_gameObject);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 全て更新
	/// </summary>
	void Update();
	
	void RegisterAll();
};
} // namespace gamelib


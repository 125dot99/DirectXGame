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
	/// �I�u�W�F�N�g��ǉ�
	/// </summary>
	/// <param name="p_gameObject"></param>
	/// <param name="groupIndex"></param>
	void Add(GameObject* p_gameObject, bool isActive = true);
	
	/// <summary>
	/// �I�u�W�F�N�g���폜
	/// </summary>
	void Remove(GameObject* p_gameObject);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �S�čX�V
	/// </summary>
	void Update();
	
	void RegisterAll();
};
} // namespace gamelib


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
	/// �I�u�W�F�N�g��ǉ�
	/// </summary>
	/// <param name="p_gameObject"></param>
	/// <param name="groupIndex"></param>
	void Add(const std::string& objectName, GameObject* pGameObject, bool isActive = true);
	
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �S�čX�V
	/// </summary>
	void Update();

	/// <summary>
	/// ���O���w�肵�ăI�u�W�F�N�g���擾
	/// </summary>
	/// <param name="objectName"></param>
	/// <returns></returns>
	GameObject* Find(const std::string& objectName) const;
	
	/// <summary>
	/// ���O�ƃC���f�b�N�X���w�肵�ăI�u�W�F�N�g���擾
	/// </summary>
	/// <param name="objectName"></param>
	/// <param name="index"></param>
	/// <returns></returns>
	GameObject* Find(const std::string& objectName, UINT index) const;
};
} // namespace gamelib


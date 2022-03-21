#include "ObjectManager.h"

void gamelib::ObjectManager::Add(const std::string& objectName, GameObject* pGameObject, bool isActive)
{
	if (pGameObject == nullptr)
	{
		return;
	}
	if (!isActive)
	{
		pGameObject->Update();
		pGameObject->SetActive(isActive);
	}
	//�I�u�W�F�N�g��ǉ�
	u_mapGameObjects[objectName].emplace_back();
	u_mapGameObjects[objectName].back().reset(pGameObject);
	//�Q�ƂɕύX
	pGameObject = u_mapGameObjects[objectName].back().get();
}

void gamelib::ObjectManager::Initialize()
{
	for (auto&& vec_gameObjects : u_mapGameObjects)
	{
		for (auto&& gameObject : vec_gameObjects.second)
		{
			gameObject->Initialize();
		}
	}
}

void gamelib::ObjectManager::Update()
{
	for (auto&& vec_gameObjects : u_mapGameObjects)
	{
		for (auto&& gameObject : vec_gameObjects.second)
		{
			gameObject->Update();
		}
	}
}

gamelib::GameObject* gamelib::ObjectManager::Find(const std::string& objectName) const
{
	if (u_mapGameObjects.find(objectName) != u_mapGameObjects.end())
	{
		return u_mapGameObjects.at(objectName).front().get();
	}
	return nullptr;
}

gamelib::GameObject* gamelib::ObjectManager::Find(const std::string& objectName, UINT index) const
{
	if (u_mapGameObjects.find(objectName) != u_mapGameObjects.end())
	{
		return u_mapGameObjects.at(objectName)[index].get();
	}
	return nullptr;
}


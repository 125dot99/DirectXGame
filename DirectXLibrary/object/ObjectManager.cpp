#include "ObjectManager.h"

gamelib::ObjectManager::ObjectManager()
{

}

gamelib::ObjectManager::~ObjectManager()
{
	for (auto it = u_map_gameObjects.begin(); it != u_map_gameObjects.end(); ++it)
	{
		for (auto gameObject : it->second)
		{
			if (gameObject)
			{
				delete gameObject;
				gameObject = nullptr;
			}
		}
	}
}

void gamelib::ObjectManager::Add(GameObject* p_gameObject, bool isActive)
{
	if (p_gameObject == nullptr)
	{
		return;
	}
	if (!isActive)
	{
		p_gameObject->Update();
		p_gameObject->SetActive(isActive);
	}
	//•ÏX‚·‚é
	u_map_gameObjects["object"].emplace_back(p_gameObject);
}

void gamelib::ObjectManager::Remove(GameObject* p_gameObject)
{
	for (auto it = u_map_gameObjects.begin(); it != u_map_gameObjects.end(); ++it)
	{
		if (u_map_gameObjects.find(it->first) != u_map_gameObjects.end())
		{
			u_map_gameObjects[it->first].erase(std::remove(u_map_gameObjects[it->first].begin(), u_map_gameObjects[it->first].end(), p_gameObject));
			return;
		}
	}
}

void gamelib::ObjectManager::Initialize()
{
	for (auto&& vec_gameObjects : u_map_gameObjects)
	{
		for (auto&& gameObject : vec_gameObjects.second)
		{
			gameObject->Initialize();
		}
	}
}

void gamelib::ObjectManager::Update()
{
	for (auto&& vec_gameObjects : u_map_gameObjects)
	{
		for (auto&& gameObject : vec_gameObjects.second)
		{
			gameObject->Update();
		}
	}
}

void gamelib::ObjectManager::RegisterAll()
{
	for (auto&& vec_gameObjects : u_map_gameObjects)
	{
		for (auto&& gameObject : vec_gameObjects.second)
		{
			gameObject->TransferBuffer();
		}
	}
}


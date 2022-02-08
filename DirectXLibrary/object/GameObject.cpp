#include "GameObject.h"

#include "../loader/ResourceManager.h"

void gamelib::GameObject::SetActive(bool _isActive)
{
	isActive = _isActive;
}

void gamelib::GameObject::SetMeshFile(const char* meshFile)
{
	mesh = ResourceManager::GetInstance()->GetModel(meshFile);
}

void gamelib::GameObject::SetParent(GameObject* pGameObject)
{
	parent = pGameObject;
}

void gamelib::GameObject::SetCollider(std::weak_ptr<BaseCollider> pCollider)
{
	collider = pCollider;
}

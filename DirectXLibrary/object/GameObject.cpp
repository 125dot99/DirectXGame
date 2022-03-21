#include "GameObject.h"

void gamelib::GameObject::SetActive(bool _isActive)
{
	isActive = _isActive;
}

void gamelib::GameObject::SetParent(GameObject* pGameObject)
{
	pParentObject = pGameObject;
}

void gamelib::GameObject::SetCollider(std::weak_ptr<BaseCollider> pCollider)
{
	w_pCollider = pCollider;
}

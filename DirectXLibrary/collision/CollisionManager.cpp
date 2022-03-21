#include "CollisionManager.h"

#include <algorithm>

gamelib::CollisionManager::CollisionManager()
{

}

void gamelib::CollisionManager::Add(GameObject* pGameObject, std::shared_ptr<BaseCollider> collider, unsigned short attribute)
{
	collider->pGameObject = pGameObject;
	collider->attribute = attribute;
	pGameObject->SetCollider(collider);
	colliders.emplace_back(collider);
}

void gamelib::CollisionManager::Remove(std::shared_ptr<BaseCollider> collider)
{
	colliders.erase(std::remove(colliders.begin(), colliders.end(), collider));
}

void gamelib::CollisionManager::ChaeckAllCollisions()
{
	int index = 0;
	const auto it_begin = colliders.cbegin();
	const auto it_end = colliders.cend();
	std::for_each(it_begin, it_end, [&](std::shared_ptr<BaseCollider> collA)
	{
		index++;
		const auto bitA = collA->attribute;
		std::for_each(it_begin + index, it_end, [&](std::shared_ptr<BaseCollider> collB)
		{
			const auto bitB = collB->attribute;
			if ((bitA & bitB) && //‚Ç‚ê‚àƒtƒ‰ƒO‚ª—§‚½‚È‚¢
				(bitA != bitB)) //‘S‚Ä“¯‚¶
			{
				if (collA->Dispatch(*collB))
				{
					collA->pGameObject->OnCollision(collA.get(), collB.get());
					collB->pGameObject->OnCollision(collB.get(), collA.get());
				}
			}
		});
	});
}

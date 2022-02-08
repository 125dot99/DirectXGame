#include "CollisionManager.h"

#include <algorithm>

#include "Collision.h"
using namespace gamelib::primitive;

bool gamelib::CollisionManager::CheckCollisions(BaseCollider* collA, BaseCollider* collB)
{
	const auto& typeA = collA->GetType();
	const auto& typeB = collB->GetType();
	if (typeA == PRIMITIVE_TYPE::BOX2D && typeB == PRIMITIVE_TYPE::BOX2D)
	{
		Box2D* boxA = dynamic_cast<Box2D*>(collA);
		Box2D* boxB = dynamic_cast<Box2D*>(collB);
		return Collision::CheckHitBox2D_Box2D(*boxA, *boxB);
	} 
	else if (typeA == PRIMITIVE_TYPE::SPHERE && typeB == PRIMITIVE_TYPE::SPHERE)
	{
		Sphere* SphereA = dynamic_cast<Sphere*>(collA);
		Sphere* SphereB = dynamic_cast<Sphere*>(collB);
		return Collision::CheckHitSphere_Sphere(*SphereA, *SphereB);
	}
	return false;
}

gamelib::CollisionManager::CollisionManager()
{

}

gamelib::CollisionManager::~CollisionManager()
{

}

void gamelib::CollisionManager::AddCollider(GameObject* gameObject, std::shared_ptr<BaseCollider> collider, unsigned short attribute)
{
	collider->gameObject = gameObject;
	collider->attribute = attribute;
	gameObject->SetCollider(collider);
	colliders.emplace_back(collider);
}

void gamelib::CollisionManager::RemoveCollider(std::shared_ptr<BaseCollider> collider)
{
	colliders.erase(std::remove(colliders.begin(), colliders.end(), collider));
}

void gamelib::CollisionManager::ChaeckAllCollisions()
{
	int index = 0;
	auto it_begin = colliders.cbegin();
	auto it_end = colliders.cend();
	std::for_each(it_begin, it_end, [&](std::shared_ptr<BaseCollider> collA)
	{
		index++;
		const auto bitA = collA.get()->attribute;
		std::for_each(it_begin + index, it_end, [&](std::shared_ptr<BaseCollider> collB)
		{
			const auto bitB = collB.get()->attribute;
			if ((bitA & bitB) && //‚Ç‚ê‚àƒtƒ‰ƒO‚ª—§‚½‚È‚¢
				(bitA != bitB)) //‘S‚Ä“¯‚¶
			{
				if (CheckCollisions(collA.get(), collB.get()))
				{
					collA->gameObject->OnCollision(collA.get(), collB.get());
					collB->gameObject->OnCollision(collB.get(), collA.get());
				}
			}
		});
	});
}

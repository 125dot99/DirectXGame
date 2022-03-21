#pragma once
#include <vector>
#include <memory>
#include "../object/GameObject.h"

namespace gamelib
{
class BaseCollider;

class CollisionManager
{
private:
	std::vector<std::shared_ptr<BaseCollider>> colliders;
public:
	CollisionManager();

	//コライダーの追加
	void Add(GameObject* pGameObject, std::shared_ptr<BaseCollider> collider, unsigned short attribute = 0xffff);
	
	//コライダーの削除
	void Remove(std::shared_ptr<BaseCollider> collider);
	
	//全てのコライダーの衝突判定を取る
	void ChaeckAllCollisions();
};
} // namespace gamelib

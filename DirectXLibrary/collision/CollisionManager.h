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

	bool CheckCollisions(BaseCollider* collA, BaseCollider* collB);
public:
	CollisionManager();
	~CollisionManager();

	//static std::unique_ptr<CollisionManager> CreateManager();

	//コライダーの追加
	void AddCollider(GameObject* gameObject, std::shared_ptr<BaseCollider> collider, unsigned short attribute = 0xffff);
	
	//コライダーの削除
	void RemoveCollider(std::shared_ptr<BaseCollider> collider);
	
	//全てのコライダーの衝突判定を取る
	void ChaeckAllCollisions();
};
} // namespace gamelib

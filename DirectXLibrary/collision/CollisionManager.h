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

	//�R���C�_�[�̒ǉ�
	void AddCollider(GameObject* gameObject, std::shared_ptr<BaseCollider> collider, unsigned short attribute = 0xffff);
	
	//�R���C�_�[�̍폜
	void RemoveCollider(std::shared_ptr<BaseCollider> collider);
	
	//�S�ẴR���C�_�[�̏Փ˔�������
	void ChaeckAllCollisions();
};
} // namespace gamelib

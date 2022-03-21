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

	//�R���C�_�[�̒ǉ�
	void Add(GameObject* pGameObject, std::shared_ptr<BaseCollider> collider, unsigned short attribute = 0xffff);
	
	//�R���C�_�[�̍폜
	void Remove(std::shared_ptr<BaseCollider> collider);
	
	//�S�ẴR���C�_�[�̏Փ˔�������
	void ChaeckAllCollisions();
};
} // namespace gamelib

#include "MiniPlayerWall.h"

MiniPlayerWall::MiniPlayerWall(HitPoint& hp) : hp(hp)
{
	hp.SetHP(3);
}

void MiniPlayerWall::Initialize()
{
	GameObject::UpdateCollider();
}

void MiniPlayerWall::Update()
{
}

void MiniPlayerWall::OnCollision(BaseCollider* collA, BaseCollider* collB)
{
	hp.Damage(1.0f);
	if (hp.IsDead())
	{
		position.y = -100;
		UpdateCollider();//����𖳌��ɂ��邽�߈�x�����Ăяo��
	}
}

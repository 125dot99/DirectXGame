#include "Mistress.h"

Mistress::Mistress(std::shared_ptr<FbxAnimation> s_pFbxAnimation, std::weak_ptr<TalkObserver> w_pTalkObserver) : 
	s_pFbxAnimation(s_pFbxAnimation), w_pTalkObserver(w_pTalkObserver)
{
	s_pFbxAnimation->Play(0);
	position = Vector3(-4, 0, 2);
}

void Mistress::Initialize()
{

}

void Mistress::Update()
{
	s_pFbxAnimation->Update();
	GameObject::Update();
}

void Mistress::OnCollision(BaseCollider* collA, BaseCollider* collB)
{
	
}

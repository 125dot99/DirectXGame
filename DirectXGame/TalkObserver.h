#pragma once
#include <vector>
#include <memory>
#include "math/Vector.h"

using namespace gamelib;

class TalkObserver
{
private:
	friend class TalkSubject;

	bool talkState;
	Vector3 direction;

	TalkSubject* pTalkSubject;
public:
	TalkObserver() = default;

	bool IsTalk() const
	{
		return talkState;
	}
	const Vector3& GetDirection() const
	{
		return direction;
	}
};

class TalkSubject
{
private:
	std::vector<std::shared_ptr<TalkObserver>> vecObservers;
public:
	//observer‚ð“o˜^
	void Register(std::shared_ptr<TalkObserver> s_pObserver)
	{
		vecObservers.emplace_back(s_pObserver);
		vecObservers.back()->pTalkSubject = this;
	}

	//’Ê’m
	void Notify(const Vector3& playerPos, const Vector3& otherPos)
	{
		for (auto&& x : vecObservers)
		{
			x->talkState = true;
		}
	}

	void Off()
	{
		for (auto&& x : vecObservers)
		{
			x->talkState = false;
		}
	}
};


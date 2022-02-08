#include "SpriteUtil.h"

#include "../math/MyMath.h"

void gamelib::SpriteAnimation::SetIndex(int stratIndex, int endIndex)
{
	count = 0;
	this->startIndex = animaIndex = stratIndex;
	this->endIndex = endIndex;
}

void gamelib::SpriteAnimation::Update(int fixedFrame)
{
	if (count++ >= fixedFrame)
	{
		count = 0;
		animaIndex = Wrap(++animaIndex, startIndex, endIndex);
	}
}

void gamelib::FontAnimation::SetIndex(int stratIndex, int endIndex)
{
	time = 0;
	animaIndex = stratIndex;
	this->endIndex = endIndex;
}

void gamelib::FontAnimation::Update(float nextAnimaSpeed)
{
	if (IsEnd())
	{
		return;
	}
	time += nextAnimaSpeed;
	if (time >= 1.0f)
	{
		time = 0;
		animaIndex = Max(endIndex, ++animaIndex);
	}
}

bool gamelib::FontAnimation::IsEnd() const
{
	return animaIndex == endIndex;
}


#include "HitPoint.h"
#include "../math/MyMath.h"

void gamelib::HitPoint::SetHP(const float point)
{
	max = hp = point;
}

void gamelib::HitPoint::Recovery(const float point)
{
	hp = Max(max, hp + point);
}

void gamelib::HitPoint::Damage(const float point)
{
	hp = Min(0.0f, hp - point);
}

const float& gamelib::HitPoint::GetHP() const
{
	return hp;
}

float gamelib::HitPoint::GetLifeGage(const float spriteSize) const
{
	return spriteSize / max * hp;
}

bool gamelib::HitPoint::IsDead() const
{
	return max - hp >= max;
}

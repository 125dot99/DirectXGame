#include "Score.h"

int Score::point = 0;

void Score::Set(const int setPoint)
{
	point = setPoint;
}

void Score::Add(const int addPoint)
{
	point += addPoint;
}

const int Score::GetScore()
{
	return point;
}

#pragma once
#include <string>

class Score
{
private:
	Score() = delete;
	~Score() = delete;
	static int point;

public:
	static void Set(const int setPoint);
	static void Add(const int addPoint);
	static const int GetScore();
};


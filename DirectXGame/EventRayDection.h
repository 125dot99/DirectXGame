#pragma once
#include "GameLibHeader.h"

using namespace gamelib;

//プレイヤーとオブジェクトの向きと距離判定を行う
class EventRayDection
{
private:
	std::vector<primitive::Sphere> vecCheckCircles;
public:
	EventRayDection();

	void Add(const Vector3& _checkPos, float radius);

	//向きと距離判定を行い、プレイヤーに近いインデックスを返す
	int Update(GameObject* pObject, float angle, float rotationOffset = 0);

	Vector3 GetEventPos(int index) const;
};

#pragma once
#include "GameLibHeader.h"

using namespace gamelib;

//�v���C���[�ƃI�u�W�F�N�g�̌����Ƌ���������s��
class EventRayDection
{
private:
	std::vector<primitive::Sphere> vecCheckCircles;
public:
	EventRayDection();

	void Add(const Vector3& _checkPos, float radius);

	//�����Ƌ���������s���A�v���C���[�ɋ߂��C���f�b�N�X��Ԃ�
	int Update(GameObject* pObject, float angle, float rotationOffset = 0);

	Vector3 GetEventPos(int index) const;
};

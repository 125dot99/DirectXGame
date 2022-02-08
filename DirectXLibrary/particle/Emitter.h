#pragma once
#include "../math/Vector.h"

namespace gamelib
{
//�p�[�e�B�N���̏����p�����[�^
struct Emitter
{
	//���[�v���邩
	bool isLoop;
	
	//�p�[�e�B�N���̔����t���[��
	int spwanFrame;
	int frame;
	
	//1�t���[���ɔ������鐔
	int spawnCount;

	//�p�[�e�B�N���̎���
	int life;
	int lifeRandFactor;

	//������
	Vector3 position;
	Vector3 positionRandFactor;

	//�����x
	Vector3 velocity;
	Vector3 velocityRandFactor;
	
	//�����x
	Vector3 accel;
	Vector3 accelRandFactor;

	Vector2 startScale;
	Vector2 startScaleRandFactor;

	Vector2 endScale;

	Vector4 startColor;
	
	Vector4 endColor;

	Emitter() : isLoop(true)
	{
		spwanFrame = 1;
		spawnCount = 1;
		life = 60;
		startColor = 1;
		startScale = 1;
	}
};
} // namespace gamelib

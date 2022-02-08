#pragma once
#include "../math/Vector.h"

namespace gamelib
{
struct Particle
{
	Vector3 position;	//���W
	Vector3 velocity;	//���x
	Vector3 accel;		//�����x

	float rotation;
	float startRotation;//�������̉�]
	float endRotation;	//���Ŏ��̉�]
	
	Vector2 scale;
	Vector2 startScale; //�������̃X�P�[��
	Vector2 endScale;	//���Ŏ��̃X�P�[��
	
	Vector4 color;
	Vector4 startColor; //�������̐F
	Vector4 endColor;	//���Ŏ��̐F
	
	int frame;
	int lifeFrame;		//�p�[�e�B�N���̎���
	
	inline Particle()
	{
		rotation = startRotation = endRotation = 0.0f;
		scale = startScale = { 1.0f, 1.0f };
		color = startColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		frame = 0;
		lifeFrame = 60;
	}

	inline Particle(const Particle& p)
	{
		*this = p;
	}
};
} // namespace gamelib
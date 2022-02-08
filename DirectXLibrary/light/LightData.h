#pragma once
#include "../math/Vector.h"

namespace gamelib
{
//���������C�g
struct DirectionalLight
{
	Vector4 direction;
	Vector4 color;
	
	DirectionalLight() : 
		direction(), 
		color(Vector4(1.0f)) {}
};

//�|�C���g���C�g
struct PointLight
{
	Vector4 position;
	Vector4 color;
	Vector4 attenuation;

	PointLight() : 
		color(Vector4(1.0f)), 
		attenuation(Vector4(1.0f)) {}
};

//�X�|�b�g���C�g
struct SpotLight
{
	Vector4 direction;
	Vector4 position;
	Vector4 color;
	Vector4 attenuation;
	Vector2 factorAngleCos;

	SpotLight() : 
		direction(),
		color(Vector4(1.0f)), 
		attenuation(Vector4(1.0f)), 
		factorAngleCos(Vector2(0.5f, 0.2f)) {}
};
} // namespcae gamelib


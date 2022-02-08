#pragma once
#include "../math/Vector.h"

namespace gamelib
{
struct Particle
{
	Vector3 position;	//座標
	Vector3 velocity;	//速度
	Vector3 accel;		//加速度

	float rotation;
	float startRotation;//生成時の回転
	float endRotation;	//消滅時の回転
	
	Vector2 scale;
	Vector2 startScale; //生成時のスケール
	Vector2 endScale;	//消滅時のスケール
	
	Vector4 color;
	Vector4 startColor; //生成時の色
	Vector4 endColor;	//消滅時の色
	
	int frame;
	int lifeFrame;		//パーティクルの寿命
	
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
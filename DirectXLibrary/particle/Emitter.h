#pragma once
#include "../math/Vector.h"

namespace gamelib
{
//パーティクルの初期パラメータ
struct Emitter
{
	//ループするか
	bool isLoop;
	
	//パーティクルの発生フレーム
	int spwanFrame;
	int frame;
	
	//1フレームに発生する数
	int spawnCount;

	//パーティクルの寿命
	int life;
	int lifeRandFactor;

	//発生源
	Vector3 position;
	Vector3 positionRandFactor;

	//初速度
	Vector3 velocity;
	Vector3 velocityRandFactor;
	
	//加速度
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

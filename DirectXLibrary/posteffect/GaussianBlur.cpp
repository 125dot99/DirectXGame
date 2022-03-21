#include "GaussianBlur.h"

void gamelib::GaussianBlur::GaussianWeights(float* blur, float sigma)
{
	float total = 0.0f;
	for (int i = 0; i < MAX_WEIGHTS; i++)
	{
		blur[i] = expf(-0.5f * (float)(i * i) / sigma);
		total += blur[i] * 2.0f;
	}
	for (int i = 0; i < MAX_WEIGHTS; i++)
	{
		blur[i] /= total;
	}
}

void gamelib::GaussianBlur::Map()
{
	GaussianWeights(gaussTable, sigma);
	ConstBufferData constMap;
	memcpy(&constMap, gaussTable, sizeof(constMap));
	cBuffer->Map(&constMap);
}

void gamelib::GaussianBlur::Init(float sigma)
{
	cBuffer.reset(new ConstBuffer);
	cBuffer->Init(0, sizeof(ConstBufferData));
	this->sigma = sigma;
	Map();
}

void gamelib::GaussianBlur::Command()
{
	cBuffer->GraphicsCommand();
}

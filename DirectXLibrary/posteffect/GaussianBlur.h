#pragma once
#include "../dx12/ConstBuffer.h"

namespace gamelib
{
//ガウシアンぼかし用のバッファ
class GaussianBlur
{
private:
	static const int MAX_WEIGHTS = 8;
	struct ConstBufferData
	{
		float gaussian[MAX_WEIGHTS];//ガウシアンぼかし
	};
	//ぼかし係数
	float sigma = 0;
	//ガウス重み
	float gaussTable[MAX_WEIGHTS]{};
	//定数バッファ
	std::unique_ptr<ConstBuffer> cBuffer;
	void GaussianWeights(float* blur, float sigma);
public:
	void Init(float sigma = 5.0f);
	void Map();
	void Command();
};

} // namespace gamelib
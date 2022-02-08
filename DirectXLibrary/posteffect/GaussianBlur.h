#pragma once
#include "../dx12/ConstBuffer.h"

namespace gamelib
{
//�K�E�V�A���ڂ����p�̃o�b�t�@
class GaussianBlur
{
private:
	static const int MAX_WEIGHTS = 8;
	struct ConstBufferData
	{
		float gaussian[MAX_WEIGHTS];//�K�E�V�A���ڂ���
	};
	//�ڂ����W��
	float sigma = 0;
	//�K�E�X�d��
	float gaussTable[MAX_WEIGHTS]{};
	//�萔�o�b�t�@
	std::unique_ptr<ConstBuffer> cBuffer;
	void GaussianWeights(float* blur, float sigma);
public:
	void Init(float sigma = 5.0f);
	void Map();
	void Command();
};

} // namespace gamelib
#pragma once
#include <string>

namespace gamelib
{
//�p�C�v���C�����N���X
class IPipelineState
{
public:
	virtual ~IPipelineState() = default;

	virtual void Command() = 0;
};

struct ShaderData
{
	std::string file; //�t�@�C����
	std::string mainEntry; //�֐���
};

enum class ROOT_PARAMETER
{
	WORLD,
	CAMERA,
	LIGHT,
	MATERIAL,
	BONE,
	TEXTURE = -1,//�e�N�X�`���������ʉ�
	CBV = WORLD,
	UAV = -2,
};
} // namespace gamelib
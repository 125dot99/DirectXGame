#pragma once
#include <string>
#include <vector>
#include <memory>
#include "ConstBuffer.h"
#include "Texture.h"
#include "../math/Vector.h"

namespace gamelib
{
class Material
{
private:
	//�萔�o�b�t�@
	std::unique_ptr<ConstBuffer> cbuffer;
public:
	//�}�e���A����
	std::string name;
	//����
	Vector3 ambient;
	//�g�U���ˌ�
	Vector3 diffuse;
	//���ʔ��ˌ�
	Vector3 specular;
	//�A���t�@
	float alpha;
	//�e�N�X�`���̃C���f�b�N�X�z��
	std::vector<std::weak_ptr<Texture>> vec_w_p_textures;
	Material();
	//�}�e���A���o�b�t�@����
	void Initialize();
	//�R�}���h(�萔�o�b�t�@)
	void Command();
};
} // namespace gamelib
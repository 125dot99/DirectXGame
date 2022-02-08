#pragma once
#include "../math/Vector.h"
namespace gamelib
{
//�}�e���A���萔�o�b�t�@�p
struct CBBasicMaterial
{
	Vector3 ambient;	//�A���r�G���g�W��
	float pad1 = 0;		//�p�f�B���O
	Vector3 diffuse;	//�f�B�t���[�Y�W��
	float pad2 = 0;		//�p�f�B���O
	Vector3 specular;	//�X�y�L�����[�W��
	float alpha;		//�A���t�@
	CBBasicMaterial(
		const Vector3& ambient,
		const Vector3& diffuse,
		const Vector3& specular,
		float alpha) :
		ambient(ambient), diffuse(diffuse), specular(specular), alpha(alpha) {}
};

//PBR�}�e���A��
struct CBPBRMaterial
{
	Vector3 basecolor;
	float metalness;	//�����x
	float specular;		//���ʔ��ˋ��x
	float roughness;	//�e��
	float pad[2];		//�p�f�B���O
};

} // namespace gamelib

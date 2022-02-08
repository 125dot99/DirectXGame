#pragma once
#include "../math/Vector.h"

namespace gamelib
{
struct VertexUv
{
	Vector3 pos; //���_���W
	Vector2 uv;	//�e�N�X�`�����W
};

struct VertexLineSizeColor
{
	Vector3 pos; //���_���W
	Vector4 color;
	float size;	//�e�N�X�`�����W
};

struct VertexSprite
{
	Vector3 pos;	//���W
	Vector2 size;	//�`��T�C�Y
	float angle;	//��]�p�x
	Vector4 color;	//�F
	Vector2 pivot;	//�X�v���C�g�̌��_
	Vector4 rect;	//�e�N�X�`����`
};

struct VertexNormalUv
{
	Vector3 pos; //���_���W
	Vector3 normal;	//�@��
	Vector2 uv;	//�e�N�X�`�����W
};

//�{�[���t���̃��b�V��
struct VertexNormalUvBones
{
	Vector3 pos; //���_���W
	Vector3 normal;	//�@��
	Vector2 uv;	//�e�N�X�`�����W
	static const int MAX_BONE_INDICES = 4; //�{�[���C���f�b�N�X�̐�
	unsigned int boneIndex[MAX_BONE_INDICES]; //�{�[���ԍ�
	float boneWeight[MAX_BONE_INDICES];	//�{�[���d��
	VertexNormalUvBones& operator=(const VertexNormalUv& v)
	{
		pos = v.pos;
		normal = v.normal;
		uv = v.uv;
		return *this;
	}
};

struct VertexNormalUvTangentBinormal
{
	Vector3 pos; //���_���W
	Vector3 normal;	//�@��
	Vector2 uv; //�e�N�X�`�����W
	Vector3 tangent; //�ڐ�
	Vector3 binormal; //�]�@��
	VertexNormalUvTangentBinormal& operator=(const VertexNormalUv& v)
	{
		pos = v.pos;
		normal = v.normal;
		uv = v.uv;
		tangent = normal.Cross(Vector3::Forward());
		binormal = normal.Cross(tangent);
		return *this;
	}
};

//�p�[�e�B�N��
struct VertexPosColorScaleAngle
{
	Vector3 pos; //���_���W
	Vector4 color; //�F
	Vector2 scale; //�X�P�[��
	float angle; //��]
};
} // namespace gamelib
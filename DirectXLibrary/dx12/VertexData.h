#pragma once
#include "../math/Vector.h"

namespace gamelib
{
struct VertexUv
{
	Vector3 pos; //頂点座標
	Vector2 uv;	//テクスチャ座標
};

struct VertexLineSizeColor
{
	Vector3 pos; //頂点座標
	Vector4 color;
	float size;	//テクスチャ座標
};

struct VertexSprite
{
	Vector3 pos;	//座標
	Vector2 size;	//描画サイズ
	float angle;	//回転角度
	Vector4 color;	//色
	Vector2 pivot;	//スプライトの原点
	Vector4 rect;	//テクスチャ矩形
};

struct VertexNormalUv
{
	Vector3 pos; //頂点座標
	Vector3 normal;	//法線
	Vector2 uv;	//テクスチャ座標
};

//ボーン付きのメッシュ
struct VertexNormalUvBones
{
	Vector3 pos; //頂点座標
	Vector3 normal;	//法線
	Vector2 uv;	//テクスチャ座標
	static const int MAX_BONE_INDICES = 4; //ボーンインデックスの数
	unsigned int boneIndex[MAX_BONE_INDICES]; //ボーン番号
	float boneWeight[MAX_BONE_INDICES];	//ボーン重み
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
	Vector3 pos; //頂点座標
	Vector3 normal;	//法線
	Vector2 uv; //テクスチャ座標
	Vector3 tangent; //接線
	Vector3 binormal; //従法線
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

//パーティクル
struct VertexPosColorScaleAngle
{
	Vector3 pos; //頂点座標
	Vector4 color; //色
	Vector2 scale; //スケール
	float angle; //回転
};
} // namespace gamelib
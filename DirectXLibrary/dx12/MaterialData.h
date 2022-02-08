#pragma once
#include "../math/Vector.h"
namespace gamelib
{
//マテリアル定数バッファ用
struct CBBasicMaterial
{
	Vector3 ambient;	//アンビエント係数
	float pad1 = 0;		//パディング
	Vector3 diffuse;	//ディフューズ係数
	float pad2 = 0;		//パディング
	Vector3 specular;	//スペキュラー係数
	float alpha;		//アルファ
	CBBasicMaterial(
		const Vector3& ambient,
		const Vector3& diffuse,
		const Vector3& specular,
		float alpha) :
		ambient(ambient), diffuse(diffuse), specular(specular), alpha(alpha) {}
};

//PBRマテリアル
struct CBPBRMaterial
{
	Vector3 basecolor;
	float metalness;	//金属度
	float specular;		//鏡面反射強度
	float roughness;	//粗さ
	float pad[2];		//パディング
};

} // namespace gamelib

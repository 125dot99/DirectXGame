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
	//定数バッファ
	std::unique_ptr<ConstBuffer> cbuffer;
public:
	//マテリアル名
	std::string name;
	//環境光
	Vector3 ambient;
	//拡散反射光
	Vector3 diffuse;
	//鏡面反射光
	Vector3 specular;
	//アルファ
	float alpha;
	//テクスチャのインデックス配列
	std::vector<std::weak_ptr<Texture>> vec_w_p_textures;
	Material();
	//マテリアルバッファ生成
	void Initialize();
	//コマンド(定数バッファ)
	void Command();
};
} // namespace gamelib
#pragma once
#include <string>
#include <memory>

#include "IMaterial.h"
#include "ConstBuffer.h"
#include "Texture.h"
#include "../math/Vector.h"
#include "../pipeline/IPipelineState.h"

namespace gamelib
{
class LambertMaterial : public IMaterial
{
private:
	//アルファ
	float alpha;
	//環境光
	Vector3 ambient;
	//拡散反射光
	Vector3 diffuse;
	//テクスチャ
	std::weak_ptr<Texture> w_pTexture;
public:
	void Create() override;
	void Register() override
	{

	}
};

class PhongMaterial : public IMaterial
{
private:
	//定数バッファ
	std::unique_ptr<ConstBuffer> u_pConstBuffer;
public:
	std::string name;
	//アルファ
	float alpha;
	//環境光
	Vector3 ambient;
	//拡散反射光
	Vector3 diffuse;
	//鏡面反射光
	Vector3 specular;
	//テクスチャ
	std::weak_ptr<Texture> w_pTexture;

	PhongMaterial() : ambient(0.5f), diffuse(0.8f), alpha(1.0f) {}

	void Create() override;

	inline void Register() override
	{
		u_pConstBuffer->GraphicsCommand();
		w_pTexture.lock()->GraphicsSRVCommand((int)ROOT_PARAMETER::MATERIAL + 1);
	}
};
} // namespace gamelib

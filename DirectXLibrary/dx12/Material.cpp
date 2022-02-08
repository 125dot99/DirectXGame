#include "Material.h"

#include "MaterialData.h"
#include "../pipeline/IPipelineState.h"


gamelib::Material::Material() : ambient(0.5f, 0.5f, 0.5f), diffuse(0.8f, 0.8f, 0.8f), alpha(1.0f)
{

}

void gamelib::Material::Initialize()
{
	cbuffer = std::make_unique<ConstBuffer>();
	cbuffer->Init((int)ROOT_PARAMETER::MATERIAL, sizeof(CBBasicMaterial));
	cbuffer->Map(&CBBasicMaterial(ambient, diffuse, specular, alpha));
}

void gamelib::Material::Command()
{
	cbuffer->GraphicsCommand();
	for (auto x : vec_w_p_textures)
	{
		x.lock()->GraphicsSRVCommand((int)ROOT_PARAMETER::MATERIAL + 1);
	}
}

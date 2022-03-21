#include "Material.h"

#include "MaterialData.h"

void gamelib::LambertMaterial::Create()
{

}

void gamelib::PhongMaterial::Create()
{
	u_pConstBuffer = std::make_unique<ConstBuffer>();
	u_pConstBuffer->Init((int)ROOT_PARAMETER::MATERIAL, sizeof(CBBasicMaterial));
	u_pConstBuffer->Map(&CBBasicMaterial(ambient, diffuse, specular, alpha));
}
#include "LightGroup.h"

#include "../pipeline/IPipelineState.h"

gamelib::LightGroup::LightGroup() : ambientLight(Vector3::One())
{
	cbuffer = std::make_unique<ConstBuffer>();
	cbuffer->Init((UINT)ROOT_PARAMETER::LIGHT, sizeof(CBLightData));
}

gamelib::LightGroup::~LightGroup()
{

}

void gamelib::LightGroup::SetAmbientLight(const Vector3& color)
{
	ambientLight = color;
}

void gamelib::LightGroup::SetDirectionalLight(DirectionalLight* directionalLight, UINT index)
{
	if (index >= CBLightData::MAX_LIGHT_BUFFER)
	{
		return;
	}
	directionalLights[index] = directionalLight;
}

void gamelib::LightGroup::SetPointLight(PointLight* pointLight, UINT index)
{
	if (index >= CBLightData::MAX_LIGHT_BUFFER)
	{
		return;
	}
	pointLights[index] = pointLight;
}

void gamelib::LightGroup::SetSpotLight(SpotLight* spotLight, UINT index)
{
	if (index >= CBLightData::MAX_LIGHT_BUFFER)
	{
		return;
	}
	spotLights[index] = spotLight;
}

void gamelib::LightGroup::Update()
{
	CBLightData lightBuffer{};
	lightBuffer.ambient = ambientLight;
	for (int i = 0; i < CBLightData::MAX_LIGHT_BUFFER; i++)
	{
		if (directionalLights[i])
		{
			lightBuffer.dirLights[i].direction = directionalLights[i]->direction;
			lightBuffer.dirLights[i].color = directionalLights[i]->color;
		}
		if (pointLights[i])
		{
			lightBuffer.pointLights[i] = *pointLights[i];
		}
		if (spotLights[i])
		{
			lightBuffer.spotLights[i] = *spotLights[i];
		}
	}
	cbuffer->Map(&lightBuffer);
}

void gamelib::LightGroup::RegisterAll()
{
	Update();
	cbuffer->GraphicsCommand();
}

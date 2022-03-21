#include "LightGroup.h"

#include "../pipeline/IPipelineState.h"

gamelib::LightGroup::LightGroup() : ambientLight(Vector3::One())
{
	u_pConstBuffer = std::make_unique<ConstBuffer>();
	u_pConstBuffer->Init((UINT)ROOT_PARAMETER::LIGHT, sizeof(CBLightData));

	vecDirectionalLights.resize(CBLightData::MAX_LIGHT_BUFFER);
	vecPointLights.resize(CBLightData::MAX_LIGHT_BUFFER);
	vecSpotLights.resize(CBLightData::MAX_LIGHT_BUFFER);
}

void gamelib::LightGroup::SetAmbientLight(const Vector3& color)
{
	ambientLight = color;
}

void gamelib::LightGroup::SetDirectionalLight(DirectionalLight* pDirectionalLight, UINT index)
{
	if (index >= CBLightData::MAX_LIGHT_BUFFER)
	{
		return;
	}
	vecDirectionalLights[index] = pDirectionalLight;
}

void gamelib::LightGroup::SetPointLight(PointLight* pPointLight, UINT index)
{
	if (index >= CBLightData::MAX_LIGHT_BUFFER)
	{
		return;
	}
	vecPointLights[index] = pPointLight;
}

void gamelib::LightGroup::SetSpotLight(SpotLight* pSpotLight, UINT index)
{
	if (index >= CBLightData::MAX_LIGHT_BUFFER)
	{
		return;
	}
	vecSpotLights[index] = pSpotLight;
}

void gamelib::LightGroup::Update()
{
	CBLightData lightBuffer{};
	lightBuffer.ambient = ambientLight;
	for (int i = 0; i < CBLightData::MAX_LIGHT_BUFFER; i++)
	{
		if (vecDirectionalLights[i])
		{
			lightBuffer.dirLights[i].direction = vecDirectionalLights[i]->direction;
			lightBuffer.dirLights[i].color = vecDirectionalLights[i]->color;
		}
		if (vecPointLights[i])
		{
			lightBuffer.pointLights[i] = *vecPointLights[i];
		}
		if (vecSpotLights[i])
		{
			lightBuffer.spotLights[i] = *vecSpotLights[i];
		}
	}
	u_pConstBuffer->Map(&lightBuffer);
}

void gamelib::LightGroup::RegisterAll()
{
	Update();
	u_pConstBuffer->GraphicsCommand();
}

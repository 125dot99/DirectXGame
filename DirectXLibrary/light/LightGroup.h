#pragma once
#include "LightData.h"
#include "../dx12/ConstBuffer.h"

namespace gamelib
{
struct CBLightData
{
	static const int MAX_LIGHT_BUFFER = 3;
	Vector4 ambient;
	DirectionalLight dirLights[MAX_LIGHT_BUFFER];
	PointLight pointLights[MAX_LIGHT_BUFFER];
	SpotLight spotLights[MAX_LIGHT_BUFFER];
};

class LightGroup
{
private:
	//環境光
	Vector3 ambientLight;
	//方向性ライト
	DirectionalLight* directionalLights[CBLightData::MAX_LIGHT_BUFFER];
	//ポイントライト
	PointLight* pointLights[CBLightData::MAX_LIGHT_BUFFER];
	//スポットライト
	SpotLight* spotLights[CBLightData::MAX_LIGHT_BUFFER];
	//ライトの定数バッファ
	std::unique_ptr<ConstBuffer> cbuffer;
public:
	LightGroup();
	~LightGroup();

	void SetAmbientLight(const Vector3& color);

	void SetDirectionalLight(DirectionalLight* directionalLight, UINT index = 0);

	void SetPointLight(PointLight* pointLight, UINT index = 0);

	void SetSpotLight(SpotLight* spotLight, UINT index = 0);

	void Update();

	void RegisterAll();
};
} // namespace gamelib


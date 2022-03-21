#pragma once
#include <vector>
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
	std::vector<DirectionalLight*> vecDirectionalLights;
	//ポイントライト
	std::vector<PointLight*> vecPointLights;
	//スポットライト
	std::vector<SpotLight*> vecSpotLights;
	//ライトの定数バッファ
	std::unique_ptr<ConstBuffer> u_pConstBuffer;
public:
	LightGroup();
	~LightGroup() = default;

	void SetAmbientLight(const Vector3& color);

	void SetDirectionalLight(DirectionalLight* pDirectionalLight, UINT index = 0);

	void SetPointLight(PointLight* pPointLight, UINT index = 0);

	void SetSpotLight(SpotLight* pSpotLight, UINT index = 0);

	void Update();

	void RegisterAll();
};
} // namespace gamelib


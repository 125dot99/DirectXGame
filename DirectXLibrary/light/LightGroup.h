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
	//����
	Vector3 ambientLight;
	//���������C�g
	DirectionalLight* directionalLights[CBLightData::MAX_LIGHT_BUFFER];
	//�|�C���g���C�g
	PointLight* pointLights[CBLightData::MAX_LIGHT_BUFFER];
	//�X�|�b�g���C�g
	SpotLight* spotLights[CBLightData::MAX_LIGHT_BUFFER];
	//���C�g�̒萔�o�b�t�@
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


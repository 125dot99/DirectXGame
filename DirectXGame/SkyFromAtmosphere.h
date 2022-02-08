#pragma once
#include <Object3D.h>
using namespace gamelib;
class SkyFromAtmosphere : public Object3D
{
private:
	struct SkyCbData : BaseCbType
	{
		Vector3 waveLength;
		float innerRadius;
		float outerRadius;
		float eSun;
		float scaleDepth;
		float kr;
		float km;
		float pad[6];		//パディング
	};
	SkyCbData cb;

	std::unique_ptr<ConstBuffer> skyCbBuff;
public:
	SkyFromAtmosphere() = default;
	~SkyFromAtmosphere() = default;
	void Initialize() override;
	void Update() override;
	void Draw() override;
};


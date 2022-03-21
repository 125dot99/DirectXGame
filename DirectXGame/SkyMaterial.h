#pragma once
#include "dx12/IMaterial.h"
#include "dx12/Texture.h"

using namespace gamelib;
class SkyMaterial : public IMaterial
{
private:
	std::weak_ptr<Texture> w_p_moonNoise;
public:
	void Create() override;
	void Register() override;
};


#pragma once
#include "dx12/ConstBuffer.h"
#include "dx12/IMaterial.h"
#include "dx12/Texture.h"

#include "math/Vector.h"
#include "math/Matrix4.h"

using namespace gamelib;

class WaveMaterial : public IMaterial
{
private:
	bool isWireFrame;
	Vector3 tessFactor;
	Matrix4 refMatrix;
	std::unique_ptr<ConstBuffer> u_pConstBuffer;
	std::weak_ptr<Texture> w_pHighTexture; //ハイトマップ
	std::weak_ptr<Texture> w_pBumpTexture; //法線
	std::weak_ptr<Texture> w_pRefTexture; //反射
public:
	void Create() override;
	void Register() override;
};


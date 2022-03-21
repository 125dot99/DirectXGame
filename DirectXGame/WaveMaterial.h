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
	std::weak_ptr<Texture> w_pHighTexture; //�n�C�g�}�b�v
	std::weak_ptr<Texture> w_pBumpTexture; //�@��
	std::weak_ptr<Texture> w_pRefTexture; //����
public:
	void Create() override;
	void Register() override;
};


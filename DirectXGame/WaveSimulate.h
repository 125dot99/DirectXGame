#pragma once
#include "object/GameObject.h"
#include "posteffect/IPostProcess.h"
#include "dx12/RenderTarget.h"
#include "dx12/ConstBuffer.h"

using namespace gamelib;

//�g���s�N�Z���V�F�[�_�[�ŃV�~�����[�g����
class WaveSimulates : public IPostProcess
{
private:
	//�g�̃T�C�Y
	Vector2 waveMeshSize;
	//�g�̔����ʒu
	GameObject* paintObject;
	std::shared_ptr<RenderTarget> renderTarget[5];
	std::unique_ptr<ConstBuffer> constBuffer;
public:
	WaveSimulates(GameObject* pPaintObject, const Vector2& _textureSize, float meshSize);
	void Befoer() override;
	void After() override;
};

class ReflectRender : public IPostProcess
{
private:
	std::shared_ptr<RenderTarget> render;
public:
	ReflectRender(const Vector2& _textureSize);
	void Befoer() override;
	void After() override;
};


#pragma once
#include "object/GameObject.h"
#include "posteffect/IPostProcess.h"
#include "dx12/RenderTarget.h"
#include "dx12/ConstBuffer.h"

using namespace gamelib;

enum WAVE_TEXTURE_ENUM
{
	INPUT_TEX,
	WAVE_PREV_TEX,
	WAVE_PREV_PREV_TEX,
	WAVE_TEX,
	NORMAL_TEX,
	REFLECT_TEX,
};

//�g���s�N�Z���V�F�[�_�[�ŃV�~�����[�g����
class WaveSimulates : public IPostProcess
{
private:
	//�g�̃T�C�Y
	Vector2 waveMeshSize;
	//�g�̔����ʒu
	GameObject* pPaintObject;
	std::shared_ptr<RenderTarget> s_pRenderTargets[5];
	std::unique_ptr<ConstBuffer> u_pConstBuffer;
public:
	WaveSimulates(GameObject* pPaintObject, const Vector2& textureSize, float meshSize);
	void Befoer() override;
	void After() override;
};

class ReflectRender : public IPostProcess
{
private:
	std::shared_ptr<RenderTarget> s_pRenderTarget;
public:
	ReflectRender(const Vector2& _textureSize);
	void Befoer() override;
	void After() override;
};


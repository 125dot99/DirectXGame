#pragma once
#include "object/GameObject.h"
#include "posteffect/IPostProcess.h"
#include "dx12/RenderTarget.h"
#include "dx12/ConstBuffer.h"

using namespace gamelib;

//波をピクセルシェーダーでシミュレートする
class WaveSimulates : public IPostProcess
{
private:
	//波のサイズ
	Vector2 waveMeshSize;
	//波の発生位置
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


#pragma once
#include "dx12/IMesh.h"
#include "dx12/ConstBuffer.h"
#include "math/Vector.h"
#include "pipeline/IPipelineState.h"
#include "object/GameObject.h"

using namespace gamelib;

class Wave
{
private:
	bool isWireFrame;
	Vector3 tessFactor;
	Matrix4 refMatrix;
	GameObject* gameObject;
	std::unique_ptr<IMesh> waveMesh;

	//テッセレーション用バッファ
	std::unique_ptr<ConstBuffer> tessBuff;

	IPipelineState* waveShader;
	IPipelineState* waveWireShader;
public:
	Wave(GameObject* pWaveObject);
	void Draw();
};


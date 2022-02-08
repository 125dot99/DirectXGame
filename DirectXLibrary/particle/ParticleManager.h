#pragma once
#include <vector>
#include <memory>
#include "ParticleUnit.h"
#include "../dx12/ConstBuffer.h"
#include "../pipeline/IPipelineState.h"

namespace gamelib
{
//パーティクルの生成管理
class ParticleManager
{
private:
	//パーティクルの配列
	std::vector<ParticleUnit*> particleUnits;
	std::unique_ptr<IPipelineState> pipeline;
public:
	ParticleManager();
	~ParticleManager();

	//受け取ったデータ元にパーティクル生成
	void Add(ParticleUnit* emitter);

	void Remove(ParticleUnit* emitter);

	void Update();

	void Draw();
};
} // class ParticleManager

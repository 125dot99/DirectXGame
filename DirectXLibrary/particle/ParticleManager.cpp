#include "ParticleManager.h"

#include "../pipeline/GraphicsPipeline.h"

gamelib::ParticleManager::ParticleManager()
{
	GraphicsPipelineDesc gpsoDesc;
	gpsoDesc.semantics = { "POSITION", "COLOR", "TEXCOORD", "PSIZE" };
	gpsoDesc.hlsl.vs.file = "ParticleVS.hlsl";
	gpsoDesc.hlsl.ps.file = "ParticlePS.hlsl";
	gpsoDesc.hlsl.gs.file = "ParticleGS.hlsl";
	gpsoDesc.blendState = BLEND_STATE::ADD;
	gpsoDesc.primitive = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	gpsoDesc.cullMode = D3D12_CULL_MODE_NONE;
	gpsoDesc.depthState = D3D12_DEPTH_WRITE_MASK_ZERO;
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD, ROOT_PARAMETER::CAMERA, ROOT_PARAMETER::LIGHT, ROOT_PARAMETER::TEXTURE };
	pipeline = std::make_unique<GraphicsPipeline>(gpsoDesc);
}

gamelib::ParticleManager::~ParticleManager()
{

}

void gamelib::ParticleManager::Add(ParticleUnit* emitter)
{
	static constexpr int MAX_EMITTER = 100;
	if (particleUnits.size() >= MAX_EMITTER)
	{
		return;
	}
	particleUnits.emplace_back(emitter);
}

void gamelib::ParticleManager::Remove(ParticleUnit* emitter)
{
	particleUnits.erase(std::remove(particleUnits.begin(), particleUnits.end(), emitter));
}

void gamelib::ParticleManager::Update()
{
	for (auto&& particle : particleUnits)
	{
		particle->Update();
	}
}

void gamelib::ParticleManager::Draw()
{
	for (auto&& particle : particleUnits)
	{
		pipeline->Command();
		particle->Draw();
	}
}

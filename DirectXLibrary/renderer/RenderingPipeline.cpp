#include "RenderingPipeline.h"

#include "../camera/Camera.h"
#include "../light/LightGroup.h"

#include "../pipeline/PipelineManager.h"

gamelib::RenderingPipeline::RenderingPipeline()
{

}

void gamelib::RenderingPipeline::SetPipeline(UINT index, const std::string& shaderName)
{
	auto shader = PipelineManager::GetInstance()->GetPipelineState(shaderName);
	u_mapPipelines[index] = shader;
}

void gamelib::RenderingPipeline::Add(UINT index, std::shared_ptr<MeshRenderer> renderer)
{
	u_mapRenderings[index].emplace_back(renderer);
}

void gamelib::RenderingPipeline::Begin()
{
	u_mapPipelines[0].lock()->Command();
}

void gamelib::RenderingPipeline::Draw()
{
	for (auto&& pair_rendering : u_mapRenderings)
	{
		u_mapPipelines[pair_rendering.first].lock()->Command();
		for (auto&& x : pair_rendering.second)
		{
			x->Draw();
		}
	}
}

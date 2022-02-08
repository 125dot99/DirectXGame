#pragma once
#include "IPipelineState.h"
#include <d3d12.h>
#include <wrl.h>
#include <vector>

namespace gamelib
{
struct ComputePipelineDesc
{
	ShaderData cs; //コンピュートシェーダー
	std::vector<ROOT_PARAMETER> rootParams;
	ComputePipelineDesc()
	{
		cs.mainEntry = "CSmain";
	}
};

//コンピュートパイプライン
class ComputePipeline : public IPipelineState
{
private:
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
public:

	ComputePipeline(ComputePipelineDesc& cpsoDesc);
	/// <summary>
	/// パイプラインのコマンド送信
	/// </summary>
	void Command() override;
};
} // namespace gamelib
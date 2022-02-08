#include "ComputePipeline.h"

#include <d3dx12.h>
#include <assert.h>

#include "HlslCompile.h"
#include "../dx12/Dx12Renderer.h"

gamelib::ComputePipeline::ComputePipeline(ComputePipelineDesc& cpsoDesc)
{
	//デスクリプタレンジ(SRV)
	std::vector<CD3DX12_DESCRIPTOR_RANGE> descRanges;
	descRanges.resize((int)cpsoDesc.rootParams.size());//一時確保しておく
	//ルートパラメータの設定(CBV, SRV)
	std::vector<CD3DX12_ROOT_PARAMETER> rootParamas;
	rootParamas.resize((int)cpsoDesc.rootParams.size());
	int rangeIndex = 0, srvCount = 0, uavCount = 0;
	for (int i = 0; i < rootParamas.size(); i++)
	{
		if (cpsoDesc.rootParams[i] == ROOT_PARAMETER::TEXTURE)
		{
			//デスクリプタレンジの初期化(SRVタイプ、デスクリプタヒープの数、レジスター番号)
			descRanges[rangeIndex].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, srvCount++);
			//SRVの割り当て(デスクリプタヒープの数、レジスター番号、全てのシェーダでアクセス可能)
			rootParamas[i].InitAsDescriptorTable(1, &descRanges[rangeIndex++]);
		}
		else if (cpsoDesc.rootParams[i] == ROOT_PARAMETER::UAV)
		{
			//デスクリプタレンジの初期化(UAVタイプ、デスクリプタヒープの数、レジスター番号)
			descRanges[rangeIndex].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, uavCount++);
			//UAVの割り当て(デスクリプタヒープの数、レジスター番号、全てのシェーダでアクセス可能)
			rootParamas[i].InitAsDescriptorTable(1, &descRanges[rangeIndex++]);
		}
		else
		{
			//CBVの割り当て(レジスター番号、 パディング、全てのシェーダでアクセス可能)
			rootParamas[i].InitAsConstantBufferView((UINT)cpsoDesc.rootParams[i]);
		}
	}

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Init((UINT)rootParamas.size(), rootParamas.data(), 0, nullptr);

	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob, errorBlob;
	HRESULT result = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	if (FAILED(result))
	{
		hlsl_compile::OutputError(errorBlob.Get());
	}
	result = Dx12Renderer::GetDevice()->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result) && "ルートシグネチャ生成失敗");

	D3D12_COMPUTE_PIPELINE_STATE_DESC cPipelineDesc{};
	//コンピュートシェーダーをセット
	cPipelineDesc.CS = hlsl_compile::CompileHlsl(cpsoDesc.cs, "cs_5_0");
	//パイプラインにルートシグネチャをセット
	cPipelineDesc.pRootSignature = rootSignature.Get();

	result = Dx12Renderer::GetDevice()->CreateComputePipelineState(
		&cPipelineDesc,
		IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result) && "コンピュートパイプライン生成失敗");
}

void gamelib::ComputePipeline::Command()
{
	//ルートシグネチャ設定のコマンド
	Dx12Renderer::GetCommandList()->SetComputeRootSignature(rootSignature.Get());
	//パイプラインステートの設定コマンド
	Dx12Renderer::GetCommandList()->SetPipelineState(pipelineState.Get());
}
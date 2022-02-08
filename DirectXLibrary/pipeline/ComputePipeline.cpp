#include "ComputePipeline.h"

#include <d3dx12.h>
#include <assert.h>

#include "HlslCompile.h"
#include "../dx12/Dx12Renderer.h"

gamelib::ComputePipeline::ComputePipeline(ComputePipelineDesc& cpsoDesc)
{
	//�f�X�N���v�^�����W(SRV)
	std::vector<CD3DX12_DESCRIPTOR_RANGE> descRanges;
	descRanges.resize((int)cpsoDesc.rootParams.size());//�ꎞ�m�ۂ��Ă���
	//���[�g�p�����[�^�̐ݒ�(CBV, SRV)
	std::vector<CD3DX12_ROOT_PARAMETER> rootParamas;
	rootParamas.resize((int)cpsoDesc.rootParams.size());
	int rangeIndex = 0, srvCount = 0, uavCount = 0;
	for (int i = 0; i < rootParamas.size(); i++)
	{
		if (cpsoDesc.rootParams[i] == ROOT_PARAMETER::TEXTURE)
		{
			//�f�X�N���v�^�����W�̏�����(SRV�^�C�v�A�f�X�N���v�^�q�[�v�̐��A���W�X�^�[�ԍ�)
			descRanges[rangeIndex].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, srvCount++);
			//SRV�̊��蓖��(�f�X�N���v�^�q�[�v�̐��A���W�X�^�[�ԍ��A�S�ẴV�F�[�_�ŃA�N�Z�X�\)
			rootParamas[i].InitAsDescriptorTable(1, &descRanges[rangeIndex++]);
		}
		else if (cpsoDesc.rootParams[i] == ROOT_PARAMETER::UAV)
		{
			//�f�X�N���v�^�����W�̏�����(UAV�^�C�v�A�f�X�N���v�^�q�[�v�̐��A���W�X�^�[�ԍ�)
			descRanges[rangeIndex].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, uavCount++);
			//UAV�̊��蓖��(�f�X�N���v�^�q�[�v�̐��A���W�X�^�[�ԍ��A�S�ẴV�F�[�_�ŃA�N�Z�X�\)
			rootParamas[i].InitAsDescriptorTable(1, &descRanges[rangeIndex++]);
		}
		else
		{
			//CBV�̊��蓖��(���W�X�^�[�ԍ��A �p�f�B���O�A�S�ẴV�F�[�_�ŃA�N�Z�X�\)
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
	assert(SUCCEEDED(result) && "���[�g�V�O�l�`���������s");

	D3D12_COMPUTE_PIPELINE_STATE_DESC cPipelineDesc{};
	//�R���s���[�g�V�F�[�_�[���Z�b�g
	cPipelineDesc.CS = hlsl_compile::CompileHlsl(cpsoDesc.cs, "cs_5_0");
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	cPipelineDesc.pRootSignature = rootSignature.Get();

	result = Dx12Renderer::GetDevice()->CreateComputePipelineState(
		&cPipelineDesc,
		IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result) && "�R���s���[�g�p�C�v���C���������s");
}

void gamelib::ComputePipeline::Command()
{
	//���[�g�V�O�l�`���ݒ�̃R�}���h
	Dx12Renderer::GetCommandList()->SetComputeRootSignature(rootSignature.Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	Dx12Renderer::GetCommandList()->SetPipelineState(pipelineState.Get());
}
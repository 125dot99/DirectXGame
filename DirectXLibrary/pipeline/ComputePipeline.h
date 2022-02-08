#pragma once
#include "IPipelineState.h"
#include <d3d12.h>
#include <wrl.h>
#include <vector>

namespace gamelib
{
struct ComputePipelineDesc
{
	ShaderData cs; //�R���s���[�g�V�F�[�_�[
	std::vector<ROOT_PARAMETER> rootParams;
	ComputePipelineDesc()
	{
		cs.mainEntry = "CSmain";
	}
};

//�R���s���[�g�p�C�v���C��
class ComputePipeline : public IPipelineState
{
private:
	//���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//�p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
public:

	ComputePipeline(ComputePipelineDesc& cpsoDesc);
	/// <summary>
	/// �p�C�v���C���̃R�}���h���M
	/// </summary>
	void Command() override;
};
} // namespace gamelib
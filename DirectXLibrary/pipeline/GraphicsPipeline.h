#pragma once
#include "IPipelineState.h"
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <vector>

namespace gamelib
{
enum class BLEND_STATE
{
	ADD, //���Z����
	INVERSE, //���]����
	SUBTRACT, //���Z����
	ALPHA //����������
};

//�͈͊O��uv�l�̋��������߂�
enum class TEXTURE_ADDRESSING_MODE
{
	WRAP, //�J��Ԃ�
	CLAMP,//0~1�͈̔͊O�ł͋߂��e�N�Z�����g����
	BORDER, //�ʓr�F�̎w�肪����
};

//�O���t�B�b�N�X�p�C�v���C���̐ݒ�
struct GraphicsShader
{
	ShaderData vs; //���_�V�F�[�_�[
	ShaderData ps; //�s�N�Z���V�F�[�_�[
	ShaderData gs; //�W�I���g���V�F�[�_�[
	ShaderData hs; //�n���V�F�[�_�[
	ShaderData ds; //�h���C���V�F�[�_�[
	inline GraphicsShader()
	{
		vs.mainEntry = "VSmain";
		ps.mainEntry = "PSmain";
		gs.mainEntry = "GSmain";
		hs.mainEntry = "HSmain";
		ds.mainEntry = "DSmain";
	}
};

//�O���t�B�b�N�X�p�C�v���C���̐ݒ�
struct GraphicsPipelineDesc
{
	BLEND_STATE blendState; //�u�����h�ݒ�(�W���͔���������)
	D3D12_FILL_MODE fillMode; //���C���[�t���[���\�����A�h��Ԃ���
	D3D12_CULL_MODE cullMode; //�J�����O���Ȃ��A�\�ʃJ�����O�A�w�ʃJ�����O
	D3D12_PRIMITIVE_TOPOLOGY_TYPE primitive; //�v���~�e�B�u�`��(�_�A���A�O�p�`�A�p�b�`)
	int renderTargetNum; //�����_�[�^�[�Q�b�g�̐�
	DXGI_FORMAT renderFormat; //�F�̃t�H�[�}�b�g�w��
	bool depthEnable; //�[�x�e�X�g���邩�ǂ���
	D3D12_DEPTH_WRITE_MASK depthState; //�[�x�o�b�t�@�֏������ޏ��
	D3D12_COMPARISON_FUNC depthFunc; //�[�x�l�̔�r
	TEXTURE_ADDRESSING_MODE textureMode; //�^�C�����O�����Ȃ���
	GraphicsShader hlsl; //�V�F�[�_�[�t�@�C���p�X
	std::vector<std::string> semantics; //���_���C�A�E�g
	std::vector<ROOT_PARAMETER> rootParams; //���[�g�p�����[�^(CBVSRV)���ԖڂɊ��蓖�Ă邩
	GraphicsPipelineDesc()
	{
		blendState = BLEND_STATE::ALPHA;
		fillMode = D3D12_FILL_MODE_SOLID;
		cullMode = D3D12_CULL_MODE_BACK;
		primitive = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		renderFormat = DXGI_FORMAT_R8G8B8A8_UNORM;//8�r�b�g
		renderTargetNum = 1;
		depthEnable = true;
		depthState = D3D12_DEPTH_WRITE_MASK_ALL;
		depthFunc = D3D12_COMPARISON_FUNC_LESS;
		textureMode = TEXTURE_ADDRESSING_MODE::WRAP;
	}
};

class GraphicsPipeline : public IPipelineState
{
private:
	//�v���~�e�B�u�`��
	D3D_PRIMITIVE_TOPOLOGY primitiveType;
	//���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//�p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

	//���_���C�A�E�g��Ԃ�
	D3D12_INPUT_ELEMENT_DESC VSInputFromSemantics(const std::string& _semanticName);
	//�u�����h�ݒ��Ԃ�
	D3D12_RENDER_TARGET_BLEND_DESC GetBlendState(BLEND_STATE blendEnum);
	//���[�g�V�O�l�`������
	void CreateRootSignature(const GraphicsPipelineDesc& _gPipeline);
public:
	/// <summary>
	/// �p�C�v���C���̐���
	/// </summary>
	GraphicsPipeline(GraphicsPipelineDesc& gPipeline);

	/// <summary>
	/// �p�C�v���C���̃R�}���h���M
	/// </summary>
	void Command() override;
};
} // namespace gamelib

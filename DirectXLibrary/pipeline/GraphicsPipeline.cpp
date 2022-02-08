#include "GraphicsPipeline.h"

#include <d3dx12.h>
#include <assert.h>
#include <map>

#include "../dx12/Dx12Renderer.h"
#include "HlslCompile.h"

#pragma comment(lib,"d3d12.lib")

DXGI_FORMAT SemanticsNameToFormat(const std::string& name)
{
	static std::map<std::string, DXGI_FORMAT> maps = {
		{ "POSITION", DXGI_FORMAT_R32G32B32_FLOAT },
		{ "NORMAL", DXGI_FORMAT_R32G32B32_FLOAT },
		{ "TEXCOORD", DXGI_FORMAT_R32G32_FLOAT },
		{ "PIVOT", DXGI_FORMAT_R32G32_FLOAT },
		{ "RECTANGLE", DXGI_FORMAT_R32G32B32A32_FLOAT },
		{ "PSIZE", DXGI_FORMAT_R32_FLOAT },
		{ "COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT },
		{ "BONEINDICES", DXGI_FORMAT_R32G32B32A32_UINT },
		{ "BONEWEIGHTS", DXGI_FORMAT_R32G32B32A32_FLOAT },
		{ "TANGENT", DXGI_FORMAT_R32G32B32_FLOAT },
		{ "BINORMAL", DXGI_FORMAT_R32G32B32_FLOAT } };
	if (maps.find(name) == maps.end())
	{
		assert(!"�Z�}���e�B�N�X�͖����ł�");
	}
	return maps[name];
}

D3D12_INPUT_ELEMENT_DESC gamelib::GraphicsPipeline::VSInputFromSemantics(const std::string& _semanticName)
{
	D3D12_INPUT_ELEMENT_DESC inputDesc{};
	std::string semantics = _semanticName;
	int back = semantics.back() - '0';
	if (back >= 0 && back <= 4)
	{
		semantics.pop_back();
		inputDesc.SemanticIndex = back;
	}
	inputDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	inputDesc.SemanticName = _semanticName.c_str();
	inputDesc.Format = SemanticsNameToFormat(semantics);
	return inputDesc;
}

D3D12_RENDER_TARGET_BLEND_DESC gamelib::GraphicsPipeline::GetBlendState(BLEND_STATE blendEnum)
{
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.BlendEnable = true; //�u�����h��L���ɂ���
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; //���Z
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE; //�\�[�X�̒l��100%�g��
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO; //�f�X�g�̒l��0%�g��
	switch (blendEnum)
	{
	case BLEND_STATE::ADD://���Z����
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_ONE;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case BLEND_STATE::INVERSE://���]����
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�`�挳
		blendDesc.DestBlend = D3D12_BLEND_ZERO;
		break;
	case BLEND_STATE::SUBTRACT://���Z����
		blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc.SrcBlend = D3D12_BLEND_ONE;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case BLEND_STATE::ALPHA://����������
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	default:
		blendEnum = BLEND_STATE::ALPHA;
	}
	return blendDesc;
}

void gamelib::GraphicsPipeline::CreateRootSignature(const GraphicsPipelineDesc& _gPipeline)
{
	int rootParamSize = (int)_gPipeline.rootParams.size();
	//�f�X�N���v�^�����W(SRV)
	std::vector<CD3DX12_DESCRIPTOR_RANGE> descRanges(rootParamSize);//�ꎞ�m�ۂ��Ă���
	//���[�g�p�����[�^�̐ݒ�(CBV, SRV)
	std::vector<CD3DX12_ROOT_PARAMETER> rootParamas(rootParamSize);
	int srvCount = 0;
	for (int i = 0; i < rootParamSize; i++)
	{
		if (_gPipeline.rootParams[i] == ROOT_PARAMETER::TEXTURE)
		{
			//�f�X�N���v�^�����W�̏�����(SRV�^�C�v�A�f�X�N���v�^�q�[�v�̐��A���W�X�^�[�ԍ�)
			descRanges[srvCount].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, srvCount);
			//SRV�̊��蓖��(�f�X�N���v�^�q�[�v�̐��A���W�X�^�[�ԍ��A�S�ẴV�F�[�_�ŃA�N�Z�X�\)
			rootParamas[i].InitAsDescriptorTable(1, &descRanges[srvCount++]);
		}
		else
		{
			//CBV�̊��蓖��(���W�X�^�[�ԍ��A �p�f�B���O�A�S�ẴV�F�[�_�ŃA�N�Z�X�\)
			rootParamas[i].InitAsConstantBufferView((UINT)_gPipeline.rootParams[i]);
		}
	}
	//�T���v���[�̐ݒ�(�e�N�X�`������ǂ̂悤�ɐF�����o�����̐ݒ�)
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);
	switch (_gPipeline.textureMode)
	{
	case TEXTURE_ADDRESSING_MODE::WRAP:
		samplerDesc.AddressU = samplerDesc.AddressV = samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		break;
	case TEXTURE_ADDRESSING_MODE::CLAMP:
		samplerDesc.AddressU = samplerDesc.AddressV = samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		break;
	case TEXTURE_ADDRESSING_MODE::BORDER:
		samplerDesc.AddressU = samplerDesc.AddressV = samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		break;
	default:
		break;
	}
	//���[�g�V�O�l�`���̐���
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
		rootParamSize,
		rootParamas.data(),
		1,
		&samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob, errorBlob;
	HRESULT result = D3DX12SerializeVersionedRootSignature(
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
	ZeroMemory(&descRanges, sizeof(descRanges));
	ZeroMemory(&rootParamas, sizeof(rootParamas));
}

gamelib::GraphicsPipeline::GraphicsPipeline(GraphicsPipelineDesc& gPipeline)
{
	//���[�g�V�O�l�`������
	CreateRootSignature(gPipeline);
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = hlsl_compile::CompileHlsl(gPipeline.hlsl.vs, "vs_5_0");
	gpipeline.PS = hlsl_compile::CompileHlsl(gPipeline.hlsl.ps, "ps_5_0");
	gpipeline.GS = hlsl_compile::CompileHlsl(gPipeline.hlsl.gs, "gs_5_0");
	gpipeline.HS = hlsl_compile::CompileHlsl(gPipeline.hlsl.hs, "hs_5_0");
	gpipeline.DS = hlsl_compile::CompileHlsl(gPipeline.hlsl.ds, "ds_5_0");
	//���_���C�A�E�g�̐ݒ�
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayouts;
	int inputLayoutSize = (int)gPipeline.semantics.size();
	for (int i = 0; i < inputLayoutSize; i++)
	{
		inputLayouts.emplace_back(VSInputFromSemantics(gPipeline.semantics[i]));
	}
	gpipeline.InputLayout.pInputElementDescs = inputLayouts.data();
	gpipeline.InputLayout.NumElements = (UINT)inputLayouts.size();
	//�T���v���}�X�N(�W���ݒ�)
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//���X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//���_�h��Ԃ�
	gpipeline.RasterizerState.FillMode = gPipeline.fillMode;
	//�w�ʃJ�����O
	gpipeline.RasterizerState.CullMode = gPipeline.cullMode;
	//�}�`�̌`��(�O�p�`)
	gpipeline.PrimitiveTopologyType = gPipeline.primitive;
	//�`��Ώ�
	gpipeline.NumRenderTargets = gPipeline.renderTargetNum;
	for (int i = 0; i < gPipeline.renderTargetNum; i++)
	{
		//�s�N�Z���V�F�[�_���o�͂��郌���_�[�^�[�Q�b�g
		gpipeline.BlendState.RenderTarget[i] = GetBlendState(gPipeline.blendState);
		//0�`255 �w���RGBA
		gpipeline.RTVFormats[i] = gPipeline.renderFormat;
	}
	//1�s�N�Z���ɂ�1��T���v�����O
	gpipeline.SampleDesc.Count = 1;
	//�[�x�o�b�t�@�̃t�H�[�}�b�g�w��
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//�[�x�o�b�t�@�̐ݒ�
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//�[�x�o�b�t�@�֏�������
	gpipeline.DepthStencilState.DepthWriteMask = gPipeline.depthState;
	//�[�x�e�X�g����
	gpipeline.DepthStencilState.DepthEnable = gPipeline.depthEnable;
	//�[�x�l�̔�r
	gpipeline.DepthStencilState.DepthFunc = gPipeline.depthFunc;
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = rootSignature.Get();
	//�p�C�v���C���X�e�[�g�̐���
	HRESULT result = Dx12Renderer::GetDevice()->CreateGraphicsPipelineState(
		&gpipeline,
		IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result) && "�O���t�B�b�N�X�p�C�v���C���������s");
	ZeroMemory(&inputLayouts, sizeof(inputLayouts));
}

void gamelib::GraphicsPipeline::Command()
{
	//���[�g�V�O�l�`���ݒ�̃R�}���h
	Dx12Renderer::GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	//�p�C�v���C���X�e�[�g�̐ݒ�R�}���h
	Dx12Renderer::GetCommandList()->SetPipelineState(pipelineState.Get());

	//Dx12Renderer::GetCommandList()->IASetPrimitiveTopology(primitiveType);
}

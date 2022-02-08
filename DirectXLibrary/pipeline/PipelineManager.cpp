#include "PipelineManager.h"

#include "GraphicsPipeline.h"
#include "ComputePipeline.h"

namespace gamelib
{
GraphicsPipelineDesc PObject3DDefalut()
{
	GraphicsPipelineDesc p{};
	p.semantics = { "POSITION", "NORMAL", "TEXCOORD" };
	p.rootParams = {
		ROOT_PARAMETER::WORLD,
		ROOT_PARAMETER::CAMERA,
		ROOT_PARAMETER::LIGHT,
		ROOT_PARAMETER::MATERIAL, 
		ROOT_PARAMETER::TEXTURE };
	return p;
}

GraphicsPipelineDesc PPosEffectDefalut()
{
	GraphicsPipelineDesc p{};
	p.semantics = { "POSITION", "TEXCOORD" };
	p.cullMode = D3D12_CULL_MODE_NONE;
	p.depthEnable = false; //�[�x�e�X�g�����Ȃ�
	return p;
}
} // namespace gamelib

gamelib::PipelineManager* gamelib::PipelineManager::GetInstance()
{
	static PipelineManager instance;
	return &instance;
}

void gamelib::PipelineManager::Initialize()
{
	ComputePipelineDesc cpsoDesc{};
	cpsoDesc.cs.file = "TestCS.hlsl";
	cpsoDesc.rootParams = { ROOT_PARAMETER::CBV, ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::UAV };
	psoDatas["CSTest"] = std::make_unique<ComputePipeline>(cpsoDesc);

#pragma region ���C�u��������

	GraphicsPipelineDesc gpsoDesc = PObject3DDefalut();
	gpsoDesc.hlsl.vs.file = "ToonVS.hlsl";
	gpsoDesc.hlsl.ps.file = "ToonPS.hlsl";
	psoDatas["ToonShader"] = std::make_unique<GraphicsPipeline>(gpsoDesc);

	//�A�E�g���C��
	gpsoDesc = GraphicsPipelineDesc();
	gpsoDesc.hlsl.vs.file = "OutlineVS.hlsl";
	gpsoDesc.hlsl.ps.file = "OutlinePS.hlsl";
	gpsoDesc.semantics = { "POSITION", "NORMAL", "TEXCOORD" };
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD, ROOT_PARAMETER::CAMERA };//transform
	gpsoDesc.cullMode = D3D12_CULL_MODE_FRONT;
	psoDatas["OutlineShader"] = std::make_unique<GraphicsPipeline>(gpsoDesc);

	//Fbx���f���̃A�E�g���C��
	gpsoDesc = GraphicsPipelineDesc();
	gpsoDesc.hlsl.vs.file = "FbxOutlineVS.hlsl";
	gpsoDesc.hlsl.ps.file = "OutlinePS.hlsl";
	gpsoDesc.semantics = { "POSITION", "NORMAL", "TEXCOORD", "BONEINDICES", "BONEWEIGHTS" };
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD, ROOT_PARAMETER::CAMERA, ROOT_PARAMETER::LIGHT,
		ROOT_PARAMETER::MATERIAL, ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::BONE };//transform
	gpsoDesc.cullMode = D3D12_CULL_MODE_FRONT;
	psoDatas["FbxOutlineShader"] = std::make_unique<GraphicsPipeline>(gpsoDesc);

	//Fbx���f��
	gpsoDesc = PObject3DDefalut();
	gpsoDesc.semantics.emplace_back("BONEINDICES");
	gpsoDesc.semantics.emplace_back("BONEWEIGHTS");
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	gpsoDesc.hlsl.vs.file = "FbxToonVS.hlsl";
	gpsoDesc.hlsl.ps.file = "FbxToonPS.hlsl";
	gpsoDesc.rootParams.emplace_back(ROOT_PARAMETER::BONE);
	psoDatas["FbxToonShader"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	gpsoDesc.hlsl.ps.file = "FbxToonSpotLightPS.hlsl";
	psoDatas["FbxToonSpotLightShader"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	gpsoDesc = GraphicsPipelineDesc();
	gpsoDesc.semantics = { "POSITION", "COLOR", "TEXCOORD", "PSIZE" };
	gpsoDesc.hlsl.vs.file = "ParticleVS.hlsl";
	gpsoDesc.hlsl.ps.file = "Particle_ToonPS.hlsl";
	gpsoDesc.hlsl.gs.file = "ParticleGS.hlsl";
	gpsoDesc.primitive = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	gpsoDesc.depthState = D3D12_DEPTH_WRITE_MASK_ZERO;
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD, ROOT_PARAMETER::CAMERA,
		ROOT_PARAMETER::LIGHT, ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::TEXTURE };
	psoDatas["ParticleToonShader"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	gpsoDesc = GraphicsPipelineDesc();
	gpsoDesc.hlsl.vs.file = "VisibleVS.hlsl";
	gpsoDesc.hlsl.ps.file = "VisiblePS.hlsl";
	gpsoDesc.semantics = { "POSITION", "NORMAL", "TEXCOORD" };
	gpsoDesc.cullMode = D3D12_CULL_MODE_NONE;
	gpsoDesc.fillMode = D3D12_FILL_MODE_WIREFRAME;
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD, ROOT_PARAMETER::CAMERA };//transform
	psoDatas["VisibleShader"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
#pragma endregion

#pragma region �|�X�g�G�t�F�N�g
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffectPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	psoDatas["PE_None"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	//�m�C�Y��p
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "NoisePS.hlsl";
	gpsoDesc.hlsl.ps.mainEntry = "CellerGray_PSmain";
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD };	//texture
	psoDatas["PE_Noise"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	//�O���[�X�P�[��
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "GrayColorPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	psoDatas["PE_Gray"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	//�Z�s�A
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "SepiaColorPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	psoDatas["PE_Sepia"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	//���U�C�N
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffect_MosaicPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	psoDatas["PE_Mosaic"] = std::make_unique<GraphicsPipeline>(gpsoDesc);

	//�O���C��
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffect_GrainPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	psoDatas["PE_Grain"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	//�K�E�V�A���ڂ��� �������ڂ���
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffect_GausswVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffect_GausswPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::CBV, ROOT_PARAMETER::TEXTURE };
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	psoDatas["PE_BlurW"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	gpsoDesc.renderFormat = DXGI_FORMAT_R32_FLOAT;
	psoDatas["PE_R32BlurW"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	//�c�����ڂ���
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffect_GausshVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffect_GausshPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::CBV, ROOT_PARAMETER::TEXTURE };
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	psoDatas["PE_BlurH"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	gpsoDesc.renderFormat = DXGI_FORMAT_R32_FLOAT;
	psoDatas["PE_R32BlurH"] = std::make_unique<GraphicsPipeline>(gpsoDesc);

	//�u���[�� ���P�x���o
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffect_HighlimPS.hlsl";
	gpsoDesc.renderFormat = DXGI_FORMAT_R8_UNORM;
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	psoDatas["PE_HighLim"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	//HDR R�����̂�
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffectPS.hlsl";
	gpsoDesc.renderFormat = DXGI_FORMAT_R32_FLOAT;
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	psoDatas["PE_R32None"] = std::make_unique<GraphicsPipeline>(gpsoDesc);

	//�u���[�� ����
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffect_BloomPS.hlsl";
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	gpsoDesc.blendState = BLEND_STATE::ADD;
	psoDatas["PE_Bloom"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	//�u���E���ǃ��j�^�[
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffect_RetroPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	psoDatas["PE_Retro"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
#pragma endregion

#pragma region �A�v��������
	//�g�p�e�b�Z���[�V����
	gpsoDesc = GraphicsPipelineDesc();
	gpsoDesc.semantics = { "POSITION", "NORMAL", "TEXCOORD" };
	gpsoDesc.hlsl.vs.file = "WaveMeshVS.hlsl";
	gpsoDesc.hlsl.ps.file = "WaveMeshPS.hlsl";
	gpsoDesc.hlsl.hs.file = "WaveMeshHS.hlsl";
	gpsoDesc.hlsl.ds.file = "WaveMeshDS.hlsl";
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	//gpsoDesc.cullMode = D3D12_CULL_MODE_NONE;
	gpsoDesc.primitive = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;//�p�b�`
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD, ROOT_PARAMETER::CAMERA, ROOT_PARAMETER::LIGHT,
		ROOT_PARAMETER::MATERIAL, ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::TEXTURE};
	psoDatas["WavePatchShader"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	//���C���[�\��
	gpsoDesc.fillMode = D3D12_FILL_MODE_WIREFRAME;
	psoDatas["WWavePatchShader"] = std::make_unique<GraphicsPipeline>(gpsoDesc);

	//��e�X�g
	gpsoDesc = PObject3DDefalut();
	gpsoDesc.hlsl.vs.file = "SkydomeVS.hlsl";
	gpsoDesc.hlsl.ps.file = "SkydomePS.hlsl";
	gpsoDesc.cullMode = D3D12_CULL_MODE_FRONT;//�ʂ�����ɂ���
	gpsoDesc.depthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDatas["SkyShader"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	//�n�C�g�}�b�v���m�[�}���}�b�v�ϊ�
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "HighNormalPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };	//texture
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	psoDatas["HighNormal"] = std::make_unique<GraphicsPipeline>(gpsoDesc);

	//�g�V�~�����[�g
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "WaveSimulatePS.hlsl";
	gpsoDesc.renderFormat = DXGI_FORMAT_R32_FLOAT;
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::TEXTURE };
	psoDatas["WaveSimulateShader"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	//���̓e�N�X�`��
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "TexPaintPS.hlsl";
	gpsoDesc.renderFormat = DXGI_FORMAT_R32_FLOAT;
	gpsoDesc.rootParams = { ROOT_PARAMETER::CBV };
	psoDatas["WavePaintShader"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
	
	//�g�V�~�����[�g�������p
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "WaveInitPS.hlsl";
	gpsoDesc.renderFormat = DXGI_FORMAT_R32_FLOAT;
	psoDatas["WaveInitShader"] = std::make_unique<GraphicsPipeline>(gpsoDesc);

	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffect_InvaderPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::TEXTURE };
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	psoDatas["PE_Inverder"] = std::make_unique<GraphicsPipeline>(gpsoDesc);
#pragma endregion
}

gamelib::IPipelineState* gamelib::PipelineManager::GetPipelineState(const std::string& _pipelineTag) const
{
	if (psoDatas.find(_pipelineTag) != psoDatas.end())
	{
		return psoDatas.at(_pipelineTag).get();
	}
	return nullptr;
}

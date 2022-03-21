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
	p.depthEnable = false; //深度テストをしない
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
	u_map_psoDatas["CSTest"] = std::make_shared<ComputePipeline>(cpsoDesc);

	cpsoDesc = {};
	cpsoDesc.cs.file = "SaveTextureCS.hlsl";
	cpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::UAV };
	u_map_psoDatas["SaveTextureCS"] = std::make_shared<ComputePipeline>(cpsoDesc);

#pragma region ライブラリ実装

	GraphicsPipelineDesc gpsoDesc = PObject3DDefalut();
	gpsoDesc.hlsl.vs.file = "ToonVS.hlsl";
	gpsoDesc.hlsl.ps.file = "ToonPS.hlsl";
	u_map_psoDatas["ToonShader"] = std::make_shared<GraphicsPipeline>(gpsoDesc);

	//アウトライン
	gpsoDesc = GraphicsPipelineDesc();
	gpsoDesc.hlsl.vs.file = "OutlineVS.hlsl";
	gpsoDesc.hlsl.ps.file = "OutlinePS.hlsl";
	gpsoDesc.semantics = { "POSITION", "NORMAL", "TEXCOORD" };
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD, ROOT_PARAMETER::CAMERA };//transform
	gpsoDesc.cullMode = D3D12_CULL_MODE_FRONT;
	u_map_psoDatas["OutlineShader"] = std::make_shared<GraphicsPipeline>(gpsoDesc);

	//Fbxモデルのアウトライン
	gpsoDesc = GraphicsPipelineDesc();
	gpsoDesc.hlsl.vs.file = "FbxOutlineVS.hlsl";
	gpsoDesc.hlsl.ps.file = "OutlinePS.hlsl";
	gpsoDesc.semantics = { "POSITION", "NORMAL", "TEXCOORD", "BONEINDICES", "BONEWEIGHTS" };
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD, ROOT_PARAMETER::CAMERA, ROOT_PARAMETER::LIGHT,
		ROOT_PARAMETER::MATERIAL, ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::BONE };//transform
	gpsoDesc.cullMode = D3D12_CULL_MODE_FRONT;
	u_map_psoDatas["FbxOutlineShader"] = std::make_shared<GraphicsPipeline>(gpsoDesc);

	//Fbxモデル
	gpsoDesc = PObject3DDefalut();
	gpsoDesc.semantics.emplace_back("BONEINDICES");
	gpsoDesc.semantics.emplace_back("BONEWEIGHTS");
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	gpsoDesc.hlsl.vs.file = "FbxToonVS.hlsl";
	gpsoDesc.hlsl.ps.file = "FbxToonPS.hlsl";
	gpsoDesc.rootParams.emplace_back(ROOT_PARAMETER::BONE);
	u_map_psoDatas["FbxToonShader"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	gpsoDesc.hlsl.ps.file = "FbxToonSpotLightPS.hlsl";
	u_map_psoDatas["FbxToonSpotLightShader"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	gpsoDesc = GraphicsPipelineDesc();
	gpsoDesc.semantics = { "POSITION", "COLOR", "TEXCOORD", "PSIZE" };
	gpsoDesc.hlsl.vs.file = "ParticleVS.hlsl";
	gpsoDesc.hlsl.ps.file = "Particle_ToonPS.hlsl";
	gpsoDesc.hlsl.gs.file = "ParticleGS.hlsl";
	gpsoDesc.primitive = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	gpsoDesc.depthState = D3D12_DEPTH_WRITE_MASK_ZERO;
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD, ROOT_PARAMETER::CAMERA,
		ROOT_PARAMETER::LIGHT, ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::TEXTURE };
	u_map_psoDatas["ParticleToonShader"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	gpsoDesc = GraphicsPipelineDesc();
	gpsoDesc.hlsl.vs.file = "VisibleVS.hlsl";
	gpsoDesc.hlsl.ps.file = "VisiblePS.hlsl";
	gpsoDesc.semantics = { "POSITION", "NORMAL", "TEXCOORD" };
	gpsoDesc.cullMode = D3D12_CULL_MODE_NONE;
	gpsoDesc.fillMode = D3D12_FILL_MODE_WIREFRAME;
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD, ROOT_PARAMETER::CAMERA };//transform
	u_map_psoDatas["VisibleShader"] = std::make_shared<GraphicsPipeline>(gpsoDesc);

	gpsoDesc = GraphicsPipelineDesc();
	gpsoDesc.hlsl.vs.file = "ShadowFBXVS.hlsl";
	gpsoDesc.hlsl.ps.file = "ShadowFBXPS.hlsl";
	gpsoDesc.semantics = { "POSITION", "NORMAL", "TEXCOORD", "BONEINDICES", "BONEWEIGHTS" };
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD, ROOT_PARAMETER::CAMERA, ROOT_PARAMETER::LIGHT, ROOT_PARAMETER::MATERIAL, 
		ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::BONE };
	u_map_psoDatas["ShadowFBX"] = std::make_shared<GraphicsPipeline>(gpsoDesc);

	gpsoDesc = GraphicsPipelineDesc();
	gpsoDesc.hlsl.vs.file = "ShadowVS.hlsl";
	gpsoDesc.hlsl.ps.file = "ShadowPS.hlsl";
	gpsoDesc.semantics = { "POSITION", "NORMAL", "TEXCOORD" };
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD, ROOT_PARAMETER::CAMERA, ROOT_PARAMETER::LIGHT, ROOT_PARAMETER::MATERIAL };
	u_map_psoDatas["Shadow"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
#pragma endregion

#pragma region ポストエフェクト
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffectPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	u_map_psoDatas["PE_None"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	//ノイズ専用
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "NoisePS.hlsl";
	gpsoDesc.hlsl.ps.mainEntry = "CellerGray_PSmain";
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD };	//texture
	u_map_psoDatas["PE_Noise"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	//グレースケール
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "GrayColorPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	u_map_psoDatas["PE_Gray"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	//セピア
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "SepiaColorPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	u_map_psoDatas["PE_Sepia"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	//モザイク
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffect_MosaicPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	u_map_psoDatas["PE_Mosaic"] = std::make_shared<GraphicsPipeline>(gpsoDesc);

	//グレイン
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffect_GrainPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	u_map_psoDatas["PE_Grain"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	//ガウシアンぼかし 横方向ぼかし
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffect_GausswVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffect_GausswPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::CBV, ROOT_PARAMETER::TEXTURE };
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	u_map_psoDatas["PE_BlurW"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	gpsoDesc.renderFormat = DXGI_FORMAT_R32_FLOAT;
	u_map_psoDatas["PE_R32BlurW"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	//縦方向ぼかし
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffect_GausshVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffect_GausshPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::CBV, ROOT_PARAMETER::TEXTURE };
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	u_map_psoDatas["PE_BlurH"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	gpsoDesc.renderFormat = DXGI_FORMAT_R32_FLOAT;
	u_map_psoDatas["PE_R32BlurH"] = std::make_shared<GraphicsPipeline>(gpsoDesc);

	//ブルーム 高輝度抽出
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffect_HighlimPS.hlsl";
	gpsoDesc.renderFormat = DXGI_FORMAT_R8_UNORM;
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	u_map_psoDatas["PE_HighLim"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	//HDR R成分のみ
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffectPS.hlsl";
	gpsoDesc.renderFormat = DXGI_FORMAT_R32_FLOAT;
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	u_map_psoDatas["PE_R32None"] = std::make_shared<GraphicsPipeline>(gpsoDesc);

	//ブルーム 合成
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffect_BloomPS.hlsl";
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	gpsoDesc.blendState = BLEND_STATE::ADD;
	u_map_psoDatas["PE_Bloom"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	//ブラウン管モニター
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffect_RetroPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	u_map_psoDatas["PE_Retro"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
#pragma endregion

#pragma region アプリ側実装
	//波用テッセレーション
	gpsoDesc = GraphicsPipelineDesc();
	gpsoDesc.semantics = { "POSITION", "NORMAL", "TEXCOORD" };
	gpsoDesc.hlsl.vs.file = "WaveMeshVS.hlsl";
	gpsoDesc.hlsl.ps.file = "WaveMeshPS.hlsl";
	gpsoDesc.hlsl.hs.file = "WaveMeshHS.hlsl";
	gpsoDesc.hlsl.ds.file = "WaveMeshDS.hlsl";
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	//gpsoDesc.cullMode = D3D12_CULL_MODE_NONE;
	gpsoDesc.primitive = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;//パッチ
	gpsoDesc.rootParams = { ROOT_PARAMETER::WORLD, ROOT_PARAMETER::CAMERA, ROOT_PARAMETER::LIGHT,
		ROOT_PARAMETER::MATERIAL, ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::TEXTURE};
	u_map_psoDatas["WavePatchShader"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	//ワイヤー表示
	gpsoDesc.fillMode = D3D12_FILL_MODE_WIREFRAME;
	u_map_psoDatas["WWavePatchShader"] = std::make_shared<GraphicsPipeline>(gpsoDesc);

	//空テスト
	gpsoDesc = PObject3DDefalut();
	gpsoDesc.hlsl.vs.file = "SkydomeVS.hlsl";
	gpsoDesc.hlsl.ps.file = "SkydomePS.hlsl";
	gpsoDesc.cullMode = D3D12_CULL_MODE_FRONT;//面を内側にする
	gpsoDesc.depthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	u_map_psoDatas["SkyShader"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	//ハイトマップをノーマルマップ変換
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "HighNormalPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE };	//texture
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	u_map_psoDatas["HighNormal"] = std::make_shared<GraphicsPipeline>(gpsoDesc);

	//波シミュレート
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "WaveSimulatePS.hlsl";
	gpsoDesc.renderFormat = DXGI_FORMAT_R32_FLOAT;
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::TEXTURE };
	u_map_psoDatas["WaveSimulateShader"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	//入力テクスチャ
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "TexPaintPS.hlsl";
	gpsoDesc.renderFormat = DXGI_FORMAT_R32_FLOAT;
	gpsoDesc.rootParams = { ROOT_PARAMETER::CBV };
	u_map_psoDatas["WavePaintShader"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
	
	//波シミュレート初期化用
	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "WaveInitPS.hlsl";
	gpsoDesc.renderFormat = DXGI_FORMAT_R32_FLOAT;
	u_map_psoDatas["WaveInitShader"] = std::make_shared<GraphicsPipeline>(gpsoDesc);

	gpsoDesc = PPosEffectDefalut();
	gpsoDesc.hlsl.vs.file = "PostEffectVS.hlsl";
	gpsoDesc.hlsl.ps.file = "PostEffect_InvaderPS.hlsl";
	gpsoDesc.rootParams = { ROOT_PARAMETER::TEXTURE, ROOT_PARAMETER::TEXTURE };
	gpsoDesc.textureMode = TEXTURE_ADDRESSING_MODE::CLAMP;
	u_map_psoDatas["PE_Inverder"] = std::make_shared<GraphicsPipeline>(gpsoDesc);
#pragma endregion
}

std::weak_ptr<gamelib::IPipelineState> gamelib::PipelineManager::GetPipelineState(const std::string& _pipelineTag) const
{
	if (u_map_psoDatas.find(_pipelineTag) != u_map_psoDatas.end())
	{
		return u_map_psoDatas.at(_pipelineTag);
	}
	return std::weak_ptr<IPipelineState>();
}

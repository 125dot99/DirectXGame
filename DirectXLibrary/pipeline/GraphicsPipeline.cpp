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
		assert(!"セマンティクスは無効です");
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
	blendDesc.BlendEnable = true; //ブレンドを有効にする
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; //加算
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE; //ソースの値を100%使う
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO; //デストの値を0%使う
	switch (blendEnum)
	{
	case BLEND_STATE::ADD://加算合成
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_ONE;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case BLEND_STATE::INVERSE://反転合成
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-描画元
		blendDesc.DestBlend = D3D12_BLEND_ZERO;
		break;
	case BLEND_STATE::SUBTRACT://減算合成
		blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc.SrcBlend = D3D12_BLEND_ONE;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case BLEND_STATE::ALPHA://半透明合成
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
	//デスクリプタレンジ(SRV)
	std::vector<CD3DX12_DESCRIPTOR_RANGE> descRanges(rootParamSize);//一時確保しておく
	//ルートパラメータの設定(CBV, SRV)
	std::vector<CD3DX12_ROOT_PARAMETER> rootParamas(rootParamSize);
	int srvCount = 0;
	for (int i = 0; i < rootParamSize; i++)
	{
		if (_gPipeline.rootParams[i] == ROOT_PARAMETER::TEXTURE)
		{
			//デスクリプタレンジの初期化(SRVタイプ、デスクリプタヒープの数、レジスター番号)
			descRanges[srvCount].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, srvCount);
			//SRVの割り当て(デスクリプタヒープの数、レジスター番号、全てのシェーダでアクセス可能)
			rootParamas[i].InitAsDescriptorTable(1, &descRanges[srvCount++]);
		}
		else
		{
			//CBVの割り当て(レジスター番号、 パディング、全てのシェーダでアクセス可能)
			rootParamas[i].InitAsConstantBufferView((UINT)_gPipeline.rootParams[i]);
		}
	}
	//サンプラーの設定(テクスチャからどのように色を取り出すかの設定)
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
	//ルートシグネチャの生成
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
	assert(SUCCEEDED(result) && "ルートシグネチャ生成失敗");
	ZeroMemory(&descRanges, sizeof(descRanges));
	ZeroMemory(&rootParamas, sizeof(rootParamas));
}

gamelib::GraphicsPipeline::GraphicsPipeline(GraphicsPipelineDesc& gPipeline)
{
	//ルートシグネチャ生成
	CreateRootSignature(gPipeline);
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = hlsl_compile::CompileHlsl(gPipeline.hlsl.vs, "vs_5_0");
	gpipeline.PS = hlsl_compile::CompileHlsl(gPipeline.hlsl.ps, "ps_5_0");
	gpipeline.GS = hlsl_compile::CompileHlsl(gPipeline.hlsl.gs, "gs_5_0");
	gpipeline.HS = hlsl_compile::CompileHlsl(gPipeline.hlsl.hs, "hs_5_0");
	gpipeline.DS = hlsl_compile::CompileHlsl(gPipeline.hlsl.ds, "ds_5_0");
	//頂点レイアウトの設定
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayouts;
	int inputLayoutSize = (int)gPipeline.semantics.size();
	for (int i = 0; i < inputLayoutSize; i++)
	{
		inputLayouts.emplace_back(VSInputFromSemantics(gPipeline.semantics[i]));
	}
	gpipeline.InputLayout.pInputElementDescs = inputLayouts.data();
	gpipeline.InputLayout.NumElements = (UINT)inputLayouts.size();
	//サンプルマスク(標準設定)
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//ラスタライザステートの設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//頂点塗りつぶし
	gpipeline.RasterizerState.FillMode = gPipeline.fillMode;
	//背面カリング
	gpipeline.RasterizerState.CullMode = gPipeline.cullMode;
	//図形の形状(三角形)
	gpipeline.PrimitiveTopologyType = gPipeline.primitive;
	//描画対象
	gpipeline.NumRenderTargets = gPipeline.renderTargetNum;
	for (int i = 0; i < gPipeline.renderTargetNum; i++)
	{
		//ピクセルシェーダが出力するレンダーターゲット
		gpipeline.BlendState.RenderTarget[i] = GetBlendState(gPipeline.blendState);
		//0～255 指定のRGBA
		gpipeline.RTVFormats[i] = gPipeline.renderFormat;
	}
	//1ピクセルにつき1回サンプリング
	gpipeline.SampleDesc.Count = 1;
	//深度バッファのフォーマット指定
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//深度バッファの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//深度バッファへ書き込む
	gpipeline.DepthStencilState.DepthWriteMask = gPipeline.depthState;
	//深度テストする
	gpipeline.DepthStencilState.DepthEnable = gPipeline.depthEnable;
	//深度値の比較
	gpipeline.DepthStencilState.DepthFunc = gPipeline.depthFunc;
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootSignature.Get();
	//パイプラインステートの生成
	HRESULT result = Dx12Renderer::GetDevice()->CreateGraphicsPipelineState(
		&gpipeline,
		IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result) && "グラフィックスパイプライン生成失敗");
	ZeroMemory(&inputLayouts, sizeof(inputLayouts));
}

void gamelib::GraphicsPipeline::Command()
{
	//ルートシグネチャ設定のコマンド
	Dx12Renderer::GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	//パイプラインステートの設定コマンド
	Dx12Renderer::GetCommandList()->SetPipelineState(pipelineState.Get());

	//Dx12Renderer::GetCommandList()->IASetPrimitiveTopology(primitiveType);
}

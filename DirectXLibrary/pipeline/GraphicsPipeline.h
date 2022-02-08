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
	ADD, //加算合成
	INVERSE, //反転合成
	SUBTRACT, //減算合成
	ALPHA //半透明合成
};

//範囲外のuv値の挙動を決める
enum class TEXTURE_ADDRESSING_MODE
{
	WRAP, //繰り返し
	CLAMP,//0~1の範囲外では近いテクセルが使われる
	BORDER, //別途色の指定がいる
};

//グラフィックスパイプラインの設定
struct GraphicsShader
{
	ShaderData vs; //頂点シェーダー
	ShaderData ps; //ピクセルシェーダー
	ShaderData gs; //ジオメトリシェーダー
	ShaderData hs; //ハルシェーダー
	ShaderData ds; //ドメインシェーダー
	inline GraphicsShader()
	{
		vs.mainEntry = "VSmain";
		ps.mainEntry = "PSmain";
		gs.mainEntry = "GSmain";
		hs.mainEntry = "HSmain";
		ds.mainEntry = "DSmain";
	}
};

//グラフィックスパイプラインの設定
struct GraphicsPipelineDesc
{
	BLEND_STATE blendState; //ブレンド設定(標準は半透明合成)
	D3D12_FILL_MODE fillMode; //ワイヤーフレーム表示か、塗りつぶすか
	D3D12_CULL_MODE cullMode; //カリングしない、表面カリング、背面カリング
	D3D12_PRIMITIVE_TOPOLOGY_TYPE primitive; //プリミティブ形状(点、線、三角形、パッチ)
	int renderTargetNum; //レンダーターゲットの数
	DXGI_FORMAT renderFormat; //色のフォーマット指定
	bool depthEnable; //深度テストするかどうか
	D3D12_DEPTH_WRITE_MASK depthState; //深度バッファへ書き込む情報
	D3D12_COMPARISON_FUNC depthFunc; //深度値の比較
	TEXTURE_ADDRESSING_MODE textureMode; //タイリングさせないか
	GraphicsShader hlsl; //シェーダーファイルパス
	std::vector<std::string> semantics; //頂点レイアウト
	std::vector<ROOT_PARAMETER> rootParams; //ルートパラメータ(CBVSRV)何番目に割り当てるか
	GraphicsPipelineDesc()
	{
		blendState = BLEND_STATE::ALPHA;
		fillMode = D3D12_FILL_MODE_SOLID;
		cullMode = D3D12_CULL_MODE_BACK;
		primitive = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		renderFormat = DXGI_FORMAT_R8G8B8A8_UNORM;//8ビット
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
	//プリミティブ形状
	D3D_PRIMITIVE_TOPOLOGY primitiveType;
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

	//頂点レイアウトを返す
	D3D12_INPUT_ELEMENT_DESC VSInputFromSemantics(const std::string& _semanticName);
	//ブレンド設定を返す
	D3D12_RENDER_TARGET_BLEND_DESC GetBlendState(BLEND_STATE blendEnum);
	//ルートシグネチャ生成
	void CreateRootSignature(const GraphicsPipelineDesc& _gPipeline);
public:
	/// <summary>
	/// パイプラインの生成
	/// </summary>
	GraphicsPipeline(GraphicsPipelineDesc& gPipeline);

	/// <summary>
	/// パイプラインのコマンド送信
	/// </summary>
	void Command() override;
};
} // namespace gamelib

#include "RenderTarget.h"

#include <d3dx12.h>
#include <assert.h>

#include "Dx12Renderer.h"
#include "../app/Application.h"

extern const float CLEAR_COLOR[4];

gamelib::RenderTexture::RenderTexture(const Vector2& _textureSize, DXGI_FORMAT format)
{
	size = _textureSize;
	if (size.Length() == 0)
	{
		size = Application::GetInstance()->GetWindowSize();
	}
	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		format,
		(UINT64)size.x,
		(UINT)size.y,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	HRESULT result = Dx12Renderer::GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&CD3DX12_CLEAR_VALUE(format, CLEAR_COLOR),
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result) && "テクスチャバッファ生成失敗");
}

void gamelib::RenderTarget::RasterizerStageCommand()
{
	//ビューポートの設定コマンド
	Dx12Renderer::GetCommandList()->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, size.x, size.y));
	//シザー矩形の設定コマンド
	Dx12Renderer::GetCommandList()->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, (LONG)size.x, (LONG)size.y));
}

void gamelib::RenderTarget::OMSetRenderTarget()
{
	//レンダーターゲット用のデスクリプタヒープのハンドルを取得
	const auto& rtvH = descriptorHeapRTV.lock()->GetCPUHandle(tableIndex);
	//レンダーターゲットのセット
	Dx12Renderer::GetCommandList()->OMSetRenderTargets(1, &rtvH, false, nullptr);
}

void gamelib::RenderTarget::OMSetRenderTargetClear()
{
	const auto& rtvH = descriptorHeapRTV.lock()->GetCPUHandle(tableIndex);
	Dx12Renderer::GetCommandList()->OMSetRenderTargets(1, &rtvH, false, nullptr);
	Dx12Renderer::GetCommandList()->ClearRenderTargetView(rtvH, CLEAR_COLOR, 0, nullptr);
}

void gamelib::RenderTarget::OMSetRenderTargetDepth()
{
	const auto& rtvH = descriptorHeapRTV.lock()->GetCPUHandle(tableIndex);
	const auto& dsvH = Dx12Renderer::GetDSVDescriptorCPUHandle();
	Dx12Renderer::GetCommandList()->OMSetRenderTargets(1, &rtvH, false, &dsvH);
}

void gamelib::RenderTarget::OMSetRenderTargetDepthClear()
{
	const auto& rtvH = descriptorHeapRTV.lock()->GetCPUHandle(tableIndex);
	const auto& dsvH = Dx12Renderer::GetDSVDescriptorCPUHandle();
	Dx12Renderer::GetCommandList()->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	Dx12Renderer::GetCommandList()->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	Dx12Renderer::GetCommandList()->ClearRenderTargetView(rtvH, CLEAR_COLOR, 0, nullptr);
}

gamelib::RenderTarget::RenderTarget(const Vector2& _textureSize, DXGI_FORMAT format)
{
	size = _textureSize;
	if (size.Length() == 0)
	{
		size = Application::GetInstance()->GetWindowSize();
	}
	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		format,
		(UINT64)size.x,
		(UINT)size.y,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	HRESULT result = Dx12Renderer::GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&CD3DX12_CLEAR_VALUE(format, CLEAR_COLOR),
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result) && "テクスチャバッファ生成失敗");
}

void gamelib::RenderTarget::CreateRTV(std::weak_ptr<DescriptorHeap> pDescHeapRTV)
{
	descriptorHeapRTV = pDescHeapRTV;
	Dx12Renderer::GetDevice()->CreateRenderTargetView(
		resource.Get(),
		nullptr,
		descriptorHeapRTV.lock()->GetCPUHandle(tableIndex));
}

void gamelib::RenderTarget::RBRenderTarget(bool isClear, bool isDepth)
{
	Dx12Renderer::GetCommandList()->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));
	RasterizerStageCommand();
	if (isClear && isDepth)
	{
		OMSetRenderTargetDepthClear();
	}
	else if (isClear && !isDepth)
	{
		OMSetRenderTargetClear();
	}
	else if (!isClear && isDepth)
	{
		OMSetRenderTargetDepth();
	}
	else
	{
		OMSetRenderTarget();
	}
}

void gamelib::RenderTarget::RBPixelShaderResource()
{
	Dx12Renderer::GetCommandList()->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}


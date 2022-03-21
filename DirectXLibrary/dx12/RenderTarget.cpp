#include "RenderTarget.h"

#include <d3dx12.h>
#include <assert.h>

#include "Dx12Renderer.h"
#include "../app/Application.h"

gamelib::RenderTarget::RenderTarget(const Vector2& textureSize, DXGI_FORMAT format, const Vector4& clearColor)
{
	clearColors[0] = clearColor.x;
	clearColors[1] = clearColor.y;
	clearColors[2] = clearColor.z;
	clearColors[3] = clearColor.w;
	size = textureSize;
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
		&CD3DX12_CLEAR_VALUE(format, clearColors),
		IID_PPV_ARGS(&com_pResource));
	assert(SUCCEEDED(result) && "テクスチャバッファ生成失敗");
}

void gamelib::RenderTarget::CreateRTV(const std::weak_ptr<DescriptorHeap>& w_pDescriptorHeapRTV)
{
	this->w_pDescriptorHeapRTV = w_pDescriptorHeapRTV;
	Dx12Renderer::GetDevice()->CreateRenderTargetView(
		com_pResource.Get(),
		nullptr,
		w_pDescriptorHeapRTV.lock()->GetCPUHandle(tableIndex));
}

void gamelib::RenderTarget::RBRenderTarget(bool isClear, bool isDepth)
{
	static auto* cmdList = Dx12Renderer::GetCommandList();
	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(com_pResource.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	//ビューポートの設定コマンド
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, size.x, size.y));
	//シザー矩形の設定コマンド
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, (LONG)size.x, (LONG)size.y));
	
	const auto& rtvH = w_pDescriptorHeapRTV.lock()->GetCPUHandle(tableIndex);
	const auto& dsvH = Dx12Renderer::GetDescriptorHespDSV()->GetCPUHandle(0);
	if (isDepth)
	{
		cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
		if (isClear)
		{
			cmdList->ClearRenderTargetView(rtvH, clearColors, 0, nullptr);
			cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		}
	}
	else
	{
		cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
		if (isClear)
		{
			cmdList->ClearRenderTargetView(rtvH, clearColors, 0, nullptr);
		}
	}
}

void gamelib::RenderTarget::RBPixelShaderResource()
{
	Dx12Renderer::GetCommandList()->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(com_pResource.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}


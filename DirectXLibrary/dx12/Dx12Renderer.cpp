#include "Dx12Renderer.h"

#include <d3dx12.h>
#include <vector>
#include <assert.h>

#include "../app/Application.h"
#include "DescriptorHeap.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")

namespace gamelib
{
template<class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
ComPtr<ID3D12Device> Dx12Renderer::com_pDevice;
ComPtr<ID3D12GraphicsCommandList> Dx12Renderer::com_pCommandList;
std::unique_ptr<DescriptorHeap> Dx12Renderer::u_pDescriptorHeapDSV;
}
extern const float CLEAR_COLOR[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

void gamelib::Dx12Renderer::OnDebugLayer()
{
#ifdef _DEBUG
	ComPtr<ID3D12Debug1> debug1;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug1))))
	{
		debug1->EnableDebugLayer();
	}
#endif
}

void gamelib::Dx12Renderer::CreateDevice()
{
	//DXGIファクトリー生成
	HRESULT result = CreateDXGIFactory1(IID_PPV_ARGS(&com_pDxgiFactory));
	assert(SUCCEEDED(result));
	std::vector<ComPtr<IDXGIAdapter>>adapters;
	ComPtr<IDXGIAdapter> tmpAdapter;
	for (int i = 0; com_pDxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.emplace_back(tmpAdapter.Get());
	}
	int adapterSize = (int)adapters.size();
	for (int i = 0; i < adapterSize; i++)
	{
		//アダプターの情報取得
		DXGI_ADAPTER_DESC adesc{};
		adapters[i]->GetDesc(&adesc);
		std::wstring strDesc = adesc.Description;
		// Microsoft Basic Render Driver,Intel UHD Graphicsを回避
		if (strDesc.find(L"Microsoft") == std::wstring::npos && 
			strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i]; // 採用
			break;
		}
	}
	//対応のDirectXデバイス
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	D3D_FEATURE_LEVEL featureLevel;
	int level = (int)_countof(levels);
	for (int i = 0; i < level; i++)
	{
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(
			tmpAdapter.Get(), 
			levels[i], 
			IID_PPV_ARGS(com_pDevice.ReleaseAndGetAddressOf()));
		if (SUCCEEDED(result))
		{
			featureLevel = levels[i];
			break;
		}
	}
}

void gamelib::Dx12Renderer::CreateSwapChain()
{
	//スワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 com_pSwapchainDesc{};
	com_pSwapchainDesc.Width = (UINT)screenSize.x;
	com_pSwapchainDesc.Height = (UINT)screenSize.y;
	com_pSwapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	com_pSwapchainDesc.SampleDesc.Count = 1;
	com_pSwapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;	//バックバッファ用
	com_pSwapchainDesc.BufferCount = FRAME_BUFFER_SIZE;
	com_pSwapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	com_pSwapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	ComPtr<IDXGISwapChain1> com_pSwapchain1;
	HRESULT result = com_pDxgiFactory->CreateSwapChainForHwnd(
		com_pCommandQueue.Get(),
		Application::GetInstance()->GetHwnd(),
		&com_pSwapchainDesc,
		nullptr,
		nullptr,
		&com_pSwapchain1);
	assert(SUCCEEDED(result) && "スワップチェーン生成失敗");
	com_pSwapchain1.As(&com_pSwapchain);
}

void gamelib::Dx12Renderer::CreateCommand()
{
	HRESULT result = com_pDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, 
		IID_PPV_ARGS(&com_pCommandAllocator));
	assert(SUCCEEDED(result) && "コマンドアロケータ生成失敗");
	
	result = com_pDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT, 
		com_pCommandAllocator.Get(),
		nullptr, 
		IID_PPV_ARGS(&com_pCommandList));
	assert(SUCCEEDED(result) && "コマンドリスト生成失敗");

	//CPUとGPUで同期をとるための仕組み
	result = com_pDevice->CreateFence(fenceValue,
		D3D12_FENCE_FLAG_NONE, 
		IID_PPV_ARGS(&com_pFence));
	assert(SUCCEEDED(result) && "フェンス生成失敗");

	//コマンドリストをGPUに順に実行していくための仕組みを生成
	result = com_pDevice->CreateCommandQueue(&D3D12_COMMAND_QUEUE_DESC(),
		IID_PPV_ARGS(&com_pCommandQueue));
	assert(SUCCEEDED(result) && "コマンドキュー生成失敗");
}

void gamelib::Dx12Renderer::CreateRenderTarget()
{
	u_pDescriptorHeapRTV.reset(new DescriptorHeap);
	u_pDescriptorHeapRTV->Create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, FRAME_BUFFER_SIZE);
	for (int i = 0; i < FRAME_BUFFER_SIZE; i++)
	{
		HRESULT result = com_pSwapchain->GetBuffer(i, IID_PPV_ARGS(&com_pBackBuffers[i]));
		assert(SUCCEEDED(result) && "スワップチェイン取得失敗");
		//レンダーターゲット用のデスクリプタヒープのハンドルを取得
		com_pDevice->CreateRenderTargetView(
			com_pBackBuffers[i].Get(),
			nullptr,
			u_pDescriptorHeapRTV->GetCPUHandle(i));
	}
}

void gamelib::Dx12Renderer::CreateDepthBuffer()
{
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		(UINT)screenSize.x,
		(UINT)screenSize.y,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	HRESULT result = com_pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&com_pDepthResource));
	assert(SUCCEEDED(result) && "深度バッファ生成失敗");
	
	//深度ビュー用デスクリプタヒープ作成
	u_pDescriptorHeapDSV = std::make_unique<DescriptorHeap>();
	u_pDescriptorHeapDSV->Create(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1);
	
	//デスクリプタヒープに深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	com_pDevice->CreateDepthStencilView(
		com_pDepthResource.Get(),
		&dsvDesc,
		u_pDescriptorHeapDSV->GetCPUHandle(0));
}

void gamelib::Dx12Renderer::Initialize()
{
	OnDebugLayer();
	screenSize = Application::GetInstance()->GetWindowSize();
	CreateDevice();
	CreateCommand();
	CreateSwapChain();
	CreateRenderTarget();
	CreateDepthBuffer();
}

void gamelib::Dx12Renderer::DrawBegin()
{
	//リソースバリアを変更
	UINT bbIendex = com_pSwapchain->GetCurrentBackBufferIndex();
	//表示状態から描画状態に変更
	com_pCommandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(com_pBackBuffers[bbIendex].Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET));
	//レンダーターゲット用のデスクリプタヒープのハンドルを取得
	const auto& rtvH = u_pDescriptorHeapRTV->GetCPUHandle(bbIendex);
	const auto& dsvH = u_pDescriptorHeapDSV->GetCPUHandle(0);
	com_pCommandList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	//ビューポートの設定コマンド
	com_pCommandList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, screenSize.x, screenSize.y));
	//シザー矩形の設定コマンド
	com_pCommandList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, (LONG)screenSize.x, (LONG)screenSize.y));
	//画面塗りつぶす
	com_pCommandList->ClearRenderTargetView(rtvH, CLEAR_COLOR, 0, nullptr);
	//深度値をクリア　RenderManager側で呼び出している
	//com_pCommandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void gamelib::Dx12Renderer::DrawEnd()
{
	UINT bbIendex = com_pSwapchain->GetCurrentBackBufferIndex();
	//リソースバリアを戻す
	com_pCommandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(com_pBackBuffers[bbIendex].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT));
	//命令のクローズ
	com_pCommandList->Close();
	//コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { com_pCommandList.Get() };
	com_pCommandQueue->ExecuteCommandLists(1, cmdLists);
	//コマンドリストの実行完了を待つ
	com_pCommandQueue->Signal(com_pFence.Get(), ++fenceValue);
	if (com_pFence->GetCompletedValue() != fenceValue)
	{
		HANDLE event = CreateEventW(nullptr, false, false, nullptr);
		com_pFence->SetEventOnCompletion(fenceValue, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	//キューをクリア
	com_pCommandAllocator->Reset();
	//再びコマンドリストを貯める準備
	com_pCommandList->Reset(com_pCommandAllocator.Get(), nullptr);
	//バッファをフリップ（裏表の入れ替え）
	com_pSwapchain->Present(1, 0);
	assert(SUCCEEDED(com_pDevice->GetDeviceRemovedReason()) && "コマンドリストのエラー");
}


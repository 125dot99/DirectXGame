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
	//DXGI�t�@�N�g���[����
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
		//�A�_�v�^�[�̏��擾
		DXGI_ADAPTER_DESC adesc{};
		adapters[i]->GetDesc(&adesc);
		std::wstring strDesc = adesc.Description;
		// Microsoft Basic Render Driver,Intel UHD Graphics�����
		if (strDesc.find(L"Microsoft") == std::wstring::npos && 
			strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i]; // �̗p
			break;
		}
	}
	//�Ή���DirectX�f�o�C�X
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
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
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
	//�X���b�v�`�F�[���𐶐�
	DXGI_SWAP_CHAIN_DESC1 com_pSwapchainDesc{};
	com_pSwapchainDesc.Width = (UINT)screenSize.x;
	com_pSwapchainDesc.Height = (UINT)screenSize.y;
	com_pSwapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	com_pSwapchainDesc.SampleDesc.Count = 1;
	com_pSwapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;	//�o�b�N�o�b�t�@�p
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
	assert(SUCCEEDED(result) && "�X���b�v�`�F�[���������s");
	com_pSwapchain1.As(&com_pSwapchain);
}

void gamelib::Dx12Renderer::CreateCommand()
{
	HRESULT result = com_pDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, 
		IID_PPV_ARGS(&com_pCommandAllocator));
	assert(SUCCEEDED(result) && "�R�}���h�A���P�[�^�������s");
	
	result = com_pDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT, 
		com_pCommandAllocator.Get(),
		nullptr, 
		IID_PPV_ARGS(&com_pCommandList));
	assert(SUCCEEDED(result) && "�R�}���h���X�g�������s");

	//CPU��GPU�œ������Ƃ邽�߂̎d�g��
	result = com_pDevice->CreateFence(fenceValue,
		D3D12_FENCE_FLAG_NONE, 
		IID_PPV_ARGS(&com_pFence));
	assert(SUCCEEDED(result) && "�t�F���X�������s");

	//�R�}���h���X�g��GPU�ɏ��Ɏ��s���Ă������߂̎d�g�݂𐶐�
	result = com_pDevice->CreateCommandQueue(&D3D12_COMMAND_QUEUE_DESC(),
		IID_PPV_ARGS(&com_pCommandQueue));
	assert(SUCCEEDED(result) && "�R�}���h�L���[�������s");
}

void gamelib::Dx12Renderer::CreateRenderTarget()
{
	u_pDescriptorHeapRTV.reset(new DescriptorHeap);
	u_pDescriptorHeapRTV->Create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, FRAME_BUFFER_SIZE);
	for (int i = 0; i < FRAME_BUFFER_SIZE; i++)
	{
		HRESULT result = com_pSwapchain->GetBuffer(i, IID_PPV_ARGS(&com_pBackBuffers[i]));
		assert(SUCCEEDED(result) && "�X���b�v�`�F�C���擾���s");
		//�����_�[�^�[�Q�b�g�p�̃f�X�N���v�^�q�[�v�̃n���h�����擾
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
	assert(SUCCEEDED(result) && "�[�x�o�b�t�@�������s");
	
	//�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	u_pDescriptorHeapDSV = std::make_unique<DescriptorHeap>();
	u_pDescriptorHeapDSV->Create(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1);
	
	//�f�X�N���v�^�q�[�v�ɐ[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
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
	//���\�[�X�o���A��ύX
	UINT bbIendex = com_pSwapchain->GetCurrentBackBufferIndex();
	//�\����Ԃ���`���ԂɕύX
	com_pCommandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(com_pBackBuffers[bbIendex].Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET));
	//�����_�[�^�[�Q�b�g�p�̃f�X�N���v�^�q�[�v�̃n���h�����擾
	const auto& rtvH = u_pDescriptorHeapRTV->GetCPUHandle(bbIendex);
	const auto& dsvH = u_pDescriptorHeapDSV->GetCPUHandle(0);
	com_pCommandList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	//�r���[�|�[�g�̐ݒ�R�}���h
	com_pCommandList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, screenSize.x, screenSize.y));
	//�V�U�[��`�̐ݒ�R�}���h
	com_pCommandList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, (LONG)screenSize.x, (LONG)screenSize.y));
	//��ʓh��Ԃ�
	com_pCommandList->ClearRenderTargetView(rtvH, CLEAR_COLOR, 0, nullptr);
	//�[�x�l���N���A�@RenderManager���ŌĂяo���Ă���
	//com_pCommandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void gamelib::Dx12Renderer::DrawEnd()
{
	UINT bbIendex = com_pSwapchain->GetCurrentBackBufferIndex();
	//���\�[�X�o���A��߂�
	com_pCommandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(com_pBackBuffers[bbIendex].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT));
	//���߂̃N���[�Y
	com_pCommandList->Close();
	//�R�}���h���X�g�̎��s
	ID3D12CommandList* cmdLists[] = { com_pCommandList.Get() };
	com_pCommandQueue->ExecuteCommandLists(1, cmdLists);
	//�R�}���h���X�g�̎��s������҂�
	com_pCommandQueue->Signal(com_pFence.Get(), ++fenceValue);
	if (com_pFence->GetCompletedValue() != fenceValue)
	{
		HANDLE event = CreateEventW(nullptr, false, false, nullptr);
		com_pFence->SetEventOnCompletion(fenceValue, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	//�L���[���N���A
	com_pCommandAllocator->Reset();
	//�ĂуR�}���h���X�g�𒙂߂鏀��
	com_pCommandList->Reset(com_pCommandAllocator.Get(), nullptr);
	//�o�b�t�@���t���b�v�i���\�̓���ւ��j
	com_pSwapchain->Present(1, 0);
	assert(SUCCEEDED(com_pDevice->GetDeviceRemovedReason()) && "�R�}���h���X�g�̃G���[");
}


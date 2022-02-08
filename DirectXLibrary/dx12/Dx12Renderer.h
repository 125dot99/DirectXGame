#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <memory>

#include "../math/Vector.h"

namespace gamelib
{
//DirectX�̊�Ղ��܂Ƃ߂��N���X
class Dx12Renderer
{
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	//�����p�f�o�C�X
	static ComPtr<ID3D12Device> device;
	//�`��R�}���h���X�g
	static ComPtr<ID3D12GraphicsCommandList> commandList;
	//�f�v�X�X�e���V���r���[�̃n���h��
	static D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
	//�E�B���h�E�T�C�Y���L���b�V��
	Vector2 screenSize;
	//DXGI�t�@�N�g���[
	ComPtr<IDXGIFactory6> dxgiFactory;
	//��ʐݒ�
	ComPtr<IDXGISwapChain4> swapchain;
	//�R�}���h�A���P�[�^
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	//�R�}���h���X�g�̑��M�A����
	ComPtr<ID3D12CommandQueue> cmdQueue;
	//�o�b�t�@�̃��\�[�X(�\�F��)
	static const char FRAME_BUFFER_SIZE = 2;
	ComPtr<ID3D12Resource> backBuffers[FRAME_BUFFER_SIZE];
	//�����_�[�^�[�Q�b�g
	std::unique_ptr<class DescriptorHeap> descriptorHeapRTV;
	//�R�}���h���X�g�̏I�����m
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceValue;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuffer;
	//�[�x�l�f�X�N���v�^�q�[�v
	std::unique_ptr<class DescriptorHeap> descriptorHeapDSV;
	//���������[�N���m�p
	void OnDebugLayer();
	void CreateDevice();
	void CreateCommand();
	void CreateSwapChain();
	void CreateRenderTarget();
	void CreateDepthBuffer();
public:
	//DirectX12�f�o�C�X�̐���
	void Initialize();
	//��ʂɕ\������Ă�����̂�����
	void DrawBegin();
	//��ʂ�\����Ԃɂ���
	void DrawEnd();
	//�ǂݎ��\�f�o�C�X
	inline static ID3D12Device* GetDevice()
	{
		return device.Get();
	}
	//�ǂݎ��\�R�}���h���X�g
	inline static ID3D12GraphicsCommandList* GetCommandList()
	{
		return commandList.Get();
	}
	inline static const D3D12_CPU_DESCRIPTOR_HANDLE& GetDSVDescriptorCPUHandle()
	{
		return dsvHandle;
	}
};
} // namespace gamelib
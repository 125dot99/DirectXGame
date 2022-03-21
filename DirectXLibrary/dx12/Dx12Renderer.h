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
	static ComPtr<ID3D12Device> com_pDevice;
	//�`��R�}���h���X�g
	static ComPtr<ID3D12GraphicsCommandList> com_pCommandList;
	//�[�x�o�b�t�@�f�X�N���v�^�q�[�v
	static std::unique_ptr<class DescriptorHeap> u_pDescriptorHeapDSV;
	//�E�B���h�E�T�C�Y���L���b�V��
	Vector2 screenSize;
	//DXGI�t�@�N�g���[
	ComPtr<IDXGIFactory6> com_pDxgiFactory;
	//��ʐݒ�
	ComPtr<IDXGISwapChain4> com_pSwapchain;
	//�R�}���h�A���P�[�^
	ComPtr<ID3D12CommandAllocator> com_pCommandAllocator;
	//�R�}���h���X�g�̑��M�A����
	ComPtr<ID3D12CommandQueue> com_pCommandQueue;
	//�o�b�t�@�̃��\�[�X(�\�F��)
	static const char FRAME_BUFFER_SIZE = 2;
	ComPtr<ID3D12Resource> com_pBackBuffers[FRAME_BUFFER_SIZE];
	//�����_�[�^�[�Q�b�g
	std::unique_ptr<class DescriptorHeap> u_pDescriptorHeapRTV;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> com_pDepthResource;
	//�R�}���h���X�g�̏I�����m
	ComPtr<ID3D12Fence> com_pFence;
	UINT64 fenceValue;
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
		return com_pDevice.Get();
	}
	//�ǂݎ��\�R�}���h���X�g
	inline static ID3D12GraphicsCommandList* GetCommandList()
	{
		return com_pCommandList.Get();
	}
	//�[�x�o�b�t�@�f�X�N���v�^�q�[�v
	inline static DescriptorHeap* GetDescriptorHespDSV()
	{
		return u_pDescriptorHeapDSV.get();
	}
};
} // namespace gamelib
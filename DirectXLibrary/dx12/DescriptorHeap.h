#pragma once
#include <d3d12.h>
#include <wrl.h>

#include "Dx12Renderer.h"

namespace gamelib
{
//�f�X�N���v�^�q�[�v�̃��b�p�[�N���X
class DescriptorHeap
{
private:
	UINT handleIncrementSize;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> com_pDescriptorHeap;
public:
	/// <summary>
	/// �f�X�N���v�^�q�[�v�̐���
	/// </summary>
	/// <param name="type">�萔�o�b�t�@�r���[(CBV)�V�F�[�_�[���\�[�X�r���[(SRV)�A�N�Z�X�r���[(UAV)�A�����_�[�^�[�Q�b�g�r���[(RTV)�A�f�v�X�X�e���V���r���[(DSV)</param>
	/// <param name="size">�q�[�v�̃T�C�Y</param>
	void Create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT size);
	
	/// <summary>
	/// �f�X�N���v�^�q�[�v�̃Z�b�g
	/// </summary>
	inline void Command()
	{
		Dx12Renderer::GetCommandList()->SetDescriptorHeaps(1, com_pDescriptorHeap.GetAddressOf());
	}
	
	/// <summary>
	/// �����I�Ȍ^�ϊ�
	/// </summary>
	inline explicit operator ID3D12DescriptorHeap*() const
	{ 
		return com_pDescriptorHeap.Get();
	}

	/// <summary>
	/// CPU�n���h�����擾����
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	inline D3D12_CPU_DESCRIPTOR_HANDLE const GetCPUHandle(UINT index)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle(com_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		handle.ptr = handle.ptr + static_cast<UINT64>(index) * static_cast<UINT64>(handleIncrementSize);
		return handle;
	}

	/// <summary>
	/// GPU�n���h�����擾����
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	inline D3D12_GPU_DESCRIPTOR_HANDLE const GetGPUHandle(UINT index)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE handle(com_pDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
		handle.ptr = handle.ptr + static_cast<UINT64>(index) * static_cast<UINT64>(handleIncrementSize);
		return handle;
	}
};
} // namespace gamelib

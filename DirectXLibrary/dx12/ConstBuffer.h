#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <assert.h>

#include "Dx12Renderer.h"

namespace gamelib
{
class ConstBuffer
{
private:
	//���[�g�p�����[�^�̓Y����
	UINT rootParamIndex;
	//�\���̂̃T�C�Y
	UINT dataSize;
	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> com_pResource;
public:
	ConstBuffer() : rootParamIndex(0), dataSize(0) {}

	/// <summary>
	/// ������
	/// </summary>
	void Init(UINT rootParamIndex, size_t u_pConstBufferDataSize)
	{
		this->rootParamIndex = rootParamIndex;

		//�q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbheapprop{};
		cbheapprop.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p

		//���\�[�X�ݒ�
		dataSize = static_cast<UINT>(u_pConstBufferDataSize);
		D3D12_RESOURCE_DESC cbresdesc{};
		cbresdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbresdesc.Width = (dataSize + 0xff) & ~0xff;//256�o�C�g�A���C�����g
		cbresdesc.Height = 1;
		cbresdesc.DepthOrArraySize = 1;
		cbresdesc.MipLevels = 1;
		cbresdesc.SampleDesc.Count = 1;
		cbresdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		//GPU���\�[�X�̐���
		HRESULT result = Dx12Renderer::GetDevice()->CreateCommittedResource(
			&cbheapprop,
			D3D12_HEAP_FLAG_NONE,
			&cbresdesc,//���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&com_pResource));
		assert(SUCCEEDED(result) && "�萔�o�b�t�@�̐������s");
	}

	/// <summary>
	/// GPU�ɒ萔�o�b�t�@��]��
	/// </summary>
	inline void Map(void* pConstBuffer)
	{
		void* constMap = nullptr;
		HRESULT result = com_pResource->Map(0, nullptr, (void**)&constMap);
		assert(SUCCEEDED(result) && "�萔�o�b�t�@�̓]�����s");
		memcpy(constMap, pConstBuffer, dataSize);
		com_pResource->Unmap(0, nullptr);
	}

	/// <summary>
	/// �O���t�B�b�N�X�p�C�v���C���̃R�}���h
	/// </summary>
	inline void GraphicsCommand() const
	{
		static auto* cmdList = Dx12Renderer::GetCommandList();
		cmdList->SetGraphicsRootConstantBufferView(rootParamIndex, com_pResource->GetGPUVirtualAddress());
	}

	/// <summary>
	/// �R���s���[�g�p�C�v���C���̃R�}���h
	/// </summary>
	inline void ComputeCommand() const
	{
		static auto* cmdList = Dx12Renderer::GetCommandList();
		cmdList->SetComputeRootConstantBufferView(rootParamIndex, com_pResource->GetGPUVirtualAddress());
	}
};
} // namespace gamelib


#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include "DescriptorHeap.h"

namespace gamelib
{
//�o�̓f�[�^���󂯎��o�b�t�@���쐬
class RWStructuredBuffer
{
private:
	//�o�̓f�[�^�̃|�C���^
	void* pOutputData;
	//�f�X�N���v�^�q�[�v�̊i�[����Ă���C���f�b�N�X
	UINT index;
	//�o�̓f�[�^�T�C�Y
	UINT dataSize;
	//�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> com_pResource;
	//UAV�f�X�N���v�^�q�[�v
	std::weak_ptr<DescriptorHeap> w_pDescriptorHeapUAV;

	void Map();
public:
	RWStructuredBuffer();

	/// <summary>
	/// UAV�𐶐�
	/// </summary>
	/// <param name="_size"></param>
	/// <param name="pDescHeap"></param>
	/// <param name="index"></param>
	void CreateUAV(const size_t& _size, std::weak_ptr<DescriptorHeap> w_pDescriptorHeapUAV, UINT index);

	/// <summary>
	/// �A���I�[�_�[�h�A�N�Z�X�r���[�̃R�}���h���M
	/// </summary>
	/// <param name="rootParamIndex"></param>
	void ComputeUAVCommand(UINT rootParamIndex);
	
	/// <summary>
	/// �R���s���[�g�V�F�[�_�[�̋N��
	/// </summary>
	/// <param name="x">�X���b�h�O���[�vx</param>
	/// <param name="y">�X���b�h�O���[�vy</param>
	/// <param name="z">�X���b�h�O���[�vz</param>
	void Dispatch(UINT x, UINT y, UINT z);
	
	/// <summary>
	/// �������񂾃o�b�t�@���擾
	/// </summary>
	/// <returns></returns>
	void* GetCSOutput() const;
};
} // namespace gamelib

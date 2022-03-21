#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <memory>

#include "../math/Vector.h"
#include "Dx12Renderer.h"
#include "DescriptorHeap.h"

namespace gamelib
{
class Texture
{
protected:
	//�ǂݎ�����Y����
	UINT tableIndex;
	//�e�N�X�`���̃T�C�Y
	Vector2 size;
	//�t�@�C����
	std::string name;
	//�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> com_pResource;
	//SRV�̃f�X�N���v�^�q�[�v
	std::weak_ptr<DescriptorHeap> w_pDescriptorHeapSRV;
public:
	Texture() = default;
	virtual ~Texture() = default;
	Texture(ID3D12Resource* pResource, const std::string& _name);
	
	/// <summary>
	/// �o�b�t�@����
	/// </summary>
	/// <param name="pDescHeap"></param>
	void CreateSRV(const std::weak_ptr<DescriptorHeap>& w_pDescriptorHeapSRV, UINT index);

	inline void GraphicsSRVCommand(UINT rootParamIndex) const
	{
		static auto* cmdList = Dx12Renderer::GetCommandList();
		w_pDescriptorHeapSRV.lock()->Command();
		cmdList->SetGraphicsRootDescriptorTable(rootParamIndex, w_pDescriptorHeapSRV.lock()->GetGPUHandle(tableIndex));
	}

	inline void ComputeSRVCommand(UINT rootParamIndex) const
	{
		static auto* cmdList = Dx12Renderer::GetCommandList();
		w_pDescriptorHeapSRV.lock()->Command();
		cmdList->SetComputeRootDescriptorTable(rootParamIndex, w_pDescriptorHeapSRV.lock()->GetGPUHandle(tableIndex));
	}

	/// <summary>
	/// �i�[����Ă���C���f�b�N�X���擾
	/// </summary>
	/// <returns></returns>
	const UINT& GetSRVIndex() const;

	/// <summary>
	/// �T�C�Y���擾
	/// </summary>
	/// <returns></returns>
	inline const Vector2& GetSize() const
	{
		return size;
	}

	/// <summary>
	/// �t�@�C�����擾
	/// </summary>
	/// <returns></returns>
	inline const std::string& GetName() const
	{
		return name;
	}
};
} // namespace gamelib

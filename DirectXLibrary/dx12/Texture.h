#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <memory>
#include "../math/Vector.h"
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
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	//SRV�̃f�X�N���v�^�q�[�v
	std::weak_ptr<DescriptorHeap> descriptorHeapSRV;
public:
	Texture() = default;
	Texture(ID3D12Resource* pResource, const std::string& _name);
	
	/// <summary>
	/// �o�b�t�@����
	/// </summary>
	/// <param name="pDescHeap"></param>
	void CreateSRV(std::weak_ptr<DescriptorHeap> pDescHeapSRV, UINT index);

	void GraphicsSRVCommand(UINT rootParamIndex) const;

	void ComputeSRVCommand(UINT rootParamIndex) const;

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

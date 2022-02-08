#include "ResourceTextureHelper.h"

#include <DirectXTex.h>
#include <d3dx12.h>
#include <assert.h>

#include "../dx12/Dx12Renderer.h"
#include "UtilityString.h"

ID3D12Resource* gamelib::ResourceTextureHelper::ReadTextureBuffer(const std::string& _fileName)
{
	DirectX::ScratchImage scratchImg;
	HRESULT result = LoadFromWICFile(
		utility_string::StrToWchar(_fileName),
		DirectX::WIC_FLAGS_NONE,
		&DirectX::TexMetadata(),
		scratchImg);
	assert(SUCCEEDED(result) && "�e�N�X�`���ǂݍ��ݎ��s");
	
	const DirectX::TexMetadata& meta = scratchImg.GetMetadata();
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		meta.format,
		(UINT64)meta.width, 
		(UINT)meta.height,
		(UINT16)meta.arraySize,
		(UINT16)meta.mipLevels);

	//�o�b�t�@����
	ID3D12Resource* textureBuffer;
	result = Dx12Renderer::GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textureBuffer));
	assert(SUCCEEDED(result) && "�e�N�X�`���o�b�t�@�������s");

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	auto* img = scratchImg.GetImage(0, 0, 0);
	result = textureBuffer->WriteToSubresource(
		0,
		nullptr,				//�S�̈�փR�s�[
		img->pixels,			//���f�[�^�A�h���X
		(UINT)img->rowPitch,	//1���C���T�C�Y
		(UINT)img->slicePitch);	//�S�T�C�Y
	assert(SUCCEEDED(result) && "�e�N�X�`���o�b�t�@�]�����s");
	return textureBuffer;
}

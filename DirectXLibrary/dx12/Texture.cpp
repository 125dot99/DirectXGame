#include "Texture.h"

gamelib::Texture::Texture(ID3D12Resource* pResource, const std::string& _name) : tableIndex(0)
{
	name = _name;
	com_pResource.Attach(pResource);
}

void gamelib::Texture::CreateSRV(const std::weak_ptr<DescriptorHeap>& w_pDescriptorHeapSRV, UINT index)
{
	tableIndex = index;
	size = Vector2((float)com_pResource.Get()->GetDesc().Width, (float)com_pResource.Get()->GetDesc().Height);
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = com_pResource->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	this->w_pDescriptorHeapSRV = w_pDescriptorHeapSRV;
	Dx12Renderer::GetDevice()->CreateShaderResourceView(
		com_pResource.Get(),
		&srvDesc,
		w_pDescriptorHeapSRV.lock()->GetCPUHandle(tableIndex));
}

const UINT& gamelib::Texture::GetSRVIndex() const
{
	return tableIndex;
}

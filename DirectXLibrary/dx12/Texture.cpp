#include "Texture.h"

#include "Dx12Renderer.h"

gamelib::Texture::Texture(ID3D12Resource* pResource, const std::string& _name) : tableIndex(0)
{
	name = _name;
	resource.Attach(pResource);
}

void gamelib::Texture::CreateSRV(std::weak_ptr<DescriptorHeap> pDescHeapSRV, UINT index)
{
	tableIndex = index;
	size = Vector2((float)resource.Get()->GetDesc().Width, (float)resource.Get()->GetDesc().Height);
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = resource->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	descriptorHeapSRV = pDescHeapSRV;
	Dx12Renderer::GetDevice()->CreateShaderResourceView(
		resource.Get(),
		&srvDesc,
		descriptorHeapSRV.lock()->GetCPUHandle(tableIndex));
}

void gamelib::Texture::GraphicsSRVCommand(UINT rootParamIndex) const
{
	static auto* cmdList = Dx12Renderer::GetCommandList();
	descriptorHeapSRV.lock()->Command();
	cmdList->SetGraphicsRootDescriptorTable(rootParamIndex, descriptorHeapSRV.lock()->GetGPUHandle(tableIndex));
}

void gamelib::Texture::ComputeSRVCommand(UINT rootParamIndex) const
{
	static auto* cmdList = Dx12Renderer::GetCommandList();
	descriptorHeapSRV.lock()->Command();
	cmdList->SetComputeRootDescriptorTable(rootParamIndex, descriptorHeapSRV.lock()->GetGPUHandle(tableIndex));
}

const UINT& gamelib::Texture::GetSRVIndex() const
{
	return tableIndex;
}

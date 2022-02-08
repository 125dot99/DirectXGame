#include "VertexBuffer.h"

#include <d3dx12.h>
#include <assert.h>

#include "Dx12Renderer.h"

// 明示的なインスタンス化
namespace gamelib 
{
template class VertexBuffer<VertexUv>;
template class VertexBuffer<VertexLineSizeColor>;
template class VertexBuffer<VertexSprite>;
template class VertexBuffer<VertexNormalUv>;
template class VertexBuffer<VertexNormalUvBones>;
template class VertexBuffer<VertexNormalUvTangentBinormal>;
template class VertexBuffer<VertexPosColorScaleAngle>;
} // namespace gamelib

template<class Vertex>
gamelib::VertexBuffer<Vertex>::VertexBuffer() : vSize(0), bufferSize(0)
{

}

template <class Vertex>
void gamelib::VertexBuffer<Vertex>::Create(UINT vertexSize, MESH_PRIMITIVE primitiveType)
{
	if (vertexSize == 0 || resource)
	{
		return;
	}
	UINT sizeVT = static_cast<UINT>(sizeof(Vertex));
	UINT sizeVB = sizeVT * static_cast<UINT>(vertexSize);
	HRESULT result = Dx12Renderer::GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result) && "頂点バッファ生成失敗");

	bufferSize = sizeVB;
	vSize = static_cast<UINT>(vertexSize);

	vBView.BufferLocation = resource->GetGPUVirtualAddress();
	vBView.SizeInBytes = sizeVB;
	vBView.StrideInBytes = sizeVT;
	this->primitiveType = primitiveType;
}

template<class Vertex>
void gamelib::VertexBuffer<Vertex>::SetPrimitive(MESH_PRIMITIVE primitive)
{
	primitiveType = primitive;
}

template <class Vertex>
void gamelib::VertexBuffer<Vertex>::Map(Vertex* pVertices)
{
	Vertex* vertMap = nullptr;
	HRESULT result = resource->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result) && "頂点バッファ転送失敗");
	memcpy(vertMap, pVertices, bufferSize);
	resource->Unmap(0, nullptr);
}

template <class Vertex>
void* gamelib::VertexBuffer<Vertex>::GetMapPointer()
{
	void* vertMap = nullptr;
	resource->Map(0, nullptr, (void**)&vertMap);
	return vertMap;
}

template <class Vertex>
void gamelib::VertexBuffer<Vertex>::UnMap()
{
	resource->Unmap(0, nullptr);
}

template <class Vertex>
void gamelib::VertexBuffer<Vertex>::BufferCommand()
{
	static auto* cmdList = Dx12Renderer::GetCommandList();
	cmdList->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)primitiveType);
	cmdList->IASetVertexBuffers(0, 1, &vBView);
}

template <class Vertex>
void gamelib::VertexBuffer<Vertex>::Draw()
{
	static auto* cmdList = Dx12Renderer::GetCommandList();
	cmdList->DrawInstanced(vSize, 1, 0, 0);
}

template<class Vertex>
void gamelib::VertexBuffer<Vertex>::Draw(UINT size, UINT startVertex)
{
	static auto* cmdList = Dx12Renderer::GetCommandList();
	cmdList->DrawInstanced(size, 1, startVertex, 0);
}


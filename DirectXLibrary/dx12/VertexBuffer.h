#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include <memory>

#include "Dx12Renderer.h"
#include "VertexData.h"

namespace gamelib
{
enum class MESH_PRIMITIVE
{
	POINT_LIST = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
	LINE_LIST = D3D_PRIMITIVE_TOPOLOGY_LINELIST,
	LINE_STRIP = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
	TRIANGLE_LIST = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	TRIANGLE_STRIP = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,

	//パッチ分割用
	POINT_3_PATCH = D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
	POINT_4_PATCH = D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
};

// テンプレート頂点バッファクラス
template <class Vertex> 
class VertexBuffer
{
private:
	// 頂点データのサイズ
	UINT vSize;
	// 頂点データ構造体のサイズ
	UINT bufferSize;
	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vBView{};
	// プリミティブ形状
	MESH_PRIMITIVE primitiveType;
public:
	// 頂点データ配列
	std::vector<Vertex> vertices;

	VertexBuffer();
	
	/// <summary>
	/// バッファの生成
	/// </summary>
	void Create(UINT vertexSize, MESH_PRIMITIVE primitiveType = MESH_PRIMITIVE::TRIANGLE_LIST);

	void SetPrimitive(MESH_PRIMITIVE primitive);

	/// <summary>
	/// バッファ転送
	/// </summary>
	/// <param name="buffer">頂点データ配列のポインタ</param>
	void Map(Vertex* pVertices);

	void* GetMapPointer();

	void UnMap();

	/// <summary>
	/// 頂点バッファの設定コマンド
	/// </summary>
	void BufferCommand();
	
	void Draw();

	void Draw(UINT size, UINT startVertex);
};
} // namespace gamelib


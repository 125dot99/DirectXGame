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

	//�p�b�`�����p
	POINT_3_PATCH = D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
	POINT_4_PATCH = D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
};

// �e���v���[�g���_�o�b�t�@�N���X
template <class Vertex> 
class VertexBuffer
{
private:
	// ���_�f�[�^�̃T�C�Y
	UINT vSize;
	// ���_�f�[�^�\���̂̃T�C�Y
	UINT bufferSize;
	// ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vBView{};
	// �v���~�e�B�u�`��
	MESH_PRIMITIVE primitiveType;
public:
	// ���_�f�[�^�z��
	std::vector<Vertex> vertices;

	VertexBuffer();
	
	/// <summary>
	/// �o�b�t�@�̐���
	/// </summary>
	void Create(UINT vertexSize, MESH_PRIMITIVE primitiveType = MESH_PRIMITIVE::TRIANGLE_LIST);

	void SetPrimitive(MESH_PRIMITIVE primitive);

	/// <summary>
	/// �o�b�t�@�]��
	/// </summary>
	/// <param name="buffer">���_�f�[�^�z��̃|�C���^</param>
	void Map(Vertex* pVertices);

	void* GetMapPointer();

	void UnMap();

	/// <summary>
	/// ���_�o�b�t�@�̐ݒ�R�}���h
	/// </summary>
	void BufferCommand();
	
	void Draw();

	void Draw(UINT size, UINT startVertex);
};
} // namespace gamelib


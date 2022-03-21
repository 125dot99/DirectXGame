#pragma once
#include <string>
#include <memory>

#include "IMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace gamelib
{
struct Mesh : IMesh
{
	std::string name;
	//���_�o�b�t�@
	std::unique_ptr<VertexBuffer<VertexNormalUv>> u_pVertexBuffer;
	//�C���f�b�N�X�o�b�t�@
	std::unique_ptr<IndexBuffer> u_pIndexBuffer;
	
	Mesh();
	~Mesh();

	void CreateBuffers() override;
	void Draw() override;
};
} // namespace gamelib
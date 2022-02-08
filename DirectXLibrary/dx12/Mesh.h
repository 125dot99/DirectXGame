#pragma once
#include "IMesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include <memory>

namespace gamelib
{
struct Mesh : IMesh
{
	std::string name;
	//頂点バッファ
	std::unique_ptr<VertexBuffer<VertexNormalUv>> vertexBuffer;
	//インデックスバッファ
	std::unique_ptr<IndexBuffer> indexBuffer;

	std::unique_ptr<Material> material;
	
	Mesh();
	~Mesh();

	void CreateBuffers() override;
	void Draw(bool isMtlCommand = true) override;
};
} // namespace gamelib
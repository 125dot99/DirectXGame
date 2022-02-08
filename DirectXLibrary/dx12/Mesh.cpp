#include "Mesh.h"

gamelib::Mesh::Mesh()
{
	vertexBuffer = std::make_unique<VertexBuffer<VertexNormalUv>>();
	indexBuffer = std::make_unique<IndexBuffer>();
}

gamelib::Mesh::~Mesh()
{

}

void gamelib::Mesh::CreateBuffers()
{
	vertexBuffer->Create(vertexBuffer->vertices.size());
	vertexBuffer->Map(vertexBuffer->vertices.data());
	indexBuffer->Create();
	if (material)
	{
		material->Initialize();
	}
}

void gamelib::Mesh::Draw(bool isMtlCommand)
{
	if (isMtlCommand && material)
	{
		material->Command();
	}
	vertexBuffer->BufferCommand();
	indexBuffer->Draw();
}

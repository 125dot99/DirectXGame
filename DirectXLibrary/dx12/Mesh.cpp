#include "Mesh.h"

gamelib::Mesh::Mesh()
{
	u_pVertexBuffer = std::make_unique<VertexBuffer<VertexNormalUv>>();
	u_pIndexBuffer = std::make_unique<IndexBuffer>();
}

gamelib::Mesh::~Mesh()
{

}

void gamelib::Mesh::CreateBuffers()
{
	u_pVertexBuffer->Create(u_pVertexBuffer->vertices.size());
	u_pVertexBuffer->Map(u_pVertexBuffer->vertices.data());
	u_pIndexBuffer->Create();
}

void gamelib::Mesh::Draw()
{
	u_pVertexBuffer->BufferCommand();
	u_pIndexBuffer->Draw();
}

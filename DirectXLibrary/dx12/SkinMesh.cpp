#include "SkinMesh.h"

gamelib::SkinMesh::SkinMesh()
{
	u_pVertexBuffer.reset(new VertexBuffer<VertexNormalUvBones>);
	u_pIndexBuffer.reset(new IndexBuffer);
}

void gamelib::SkinMesh::CreateBuffers()
{
	u_pVertexBuffer->Create((UINT)u_pVertexBuffer->vertices.size());
	u_pVertexBuffer->Map(u_pVertexBuffer->vertices.data());
	u_pIndexBuffer->Create();
}

void gamelib::SkinMesh::Draw()
{
	u_pVertexBuffer->BufferCommand();
	u_pIndexBuffer->Draw();
}
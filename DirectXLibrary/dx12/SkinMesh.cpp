#include "SkinMesh.h"

gamelib::SkinMesh::SkinMesh()
{
	vertexBuffer.reset(new VertexBuffer<VertexNormalUvBones>);
	indexBuffer.reset(new IndexBuffer);
}

gamelib::SkinMesh::~SkinMesh()
{

}

void gamelib::SkinMesh::CreateBuffers()
{
	vertexBuffer->Create(vertexBuffer->vertices.size());
	vertexBuffer->Map(vertexBuffer->vertices.data());
	indexBuffer->Create();
	if (material)
	{
		material->Initialize();
	}
}

void gamelib::SkinMesh::Draw(bool isMtlCommand)
{
	if (isMtlCommand && material)
	{
		material->Command();
	}
	vertexBuffer->BufferCommand();
	indexBuffer->Draw();
}
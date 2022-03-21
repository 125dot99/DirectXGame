#include "ColliderData.h"

#include "../dx12/Mesh.h"

gamelib::MeshCollider::MeshCollider(std::weak_ptr<IMesh> mesh)
{
	Mesh* pmesh = dynamic_cast<Mesh*>(mesh.lock().get());
	const auto& vertices = pmesh->u_pVertexBuffer->vertices;
	const auto& indices = pmesh->u_pIndexBuffer->indices;
	const int triangleSize = (int)indices.size() / 3;
	for (int i = 0; i < triangleSize; i++)
	{
		primitive::Triangle& tri = triangles[i];
		int index0 = indices[(INT64)i * 3 + 0];
		int index1 = indices[(INT64)i * 3 + 1];
		int index2 = indices[(INT64)i * 3 + 2];

		tri.p0 = vertices[index0].pos;
		tri.p1 = vertices[index1].pos;
		tri.p2 = vertices[index2].pos;
		tri.ComputeNormal();
	}
}

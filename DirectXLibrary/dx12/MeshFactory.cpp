#include "MeshFactory.h"
#include "Mesh.h"

void gamelib::MeshFactory::ComputeNormalFlat(IMesh* pMesh)
{
    Mesh* mesh = dynamic_cast<Mesh*>(pMesh);
    std::vector<VertexNormalUv>& vertices = mesh->u_pVertexBuffer->vertices;
    std::vector<unsigned short>& indices = mesh->u_pIndexBuffer->indices;
    for (int i = 0; i < indices.size() / 3; i++)
    {
        int index = i * 3;
        unsigned short i0 = indices[static_cast<size_t>(index) + 0];
        unsigned short i1 = indices[static_cast<size_t>(index) + 1];
        unsigned short i2 = indices[static_cast<size_t>(index) + 2];
        //三角形を構成する頂点座標をベクトルに代入
        Vector3 p0 = vertices[i0].pos;
        Vector3 p1 = vertices[i1].pos;
        Vector3 p2 = vertices[i2].pos;
        //p0->p1ベクトル、p0->p2ベクトルを計算(ベクトルの減算)
        Vector3 v1 = p1 - p0;
        Vector3 v2 = p2 - p0;
        //外積は両方から垂直なベクトル
        Vector3 normal = v1.Cross(v2);
        //正規化(長さを1にする)
        normal.Normalize();
        //求めた法線を頂点データに代入
        vertices[i0].normal = normal;
        vertices[i1].normal = normal;
        vertices[i2].normal = normal;
    }
}

void gamelib::MeshFactory::ComputeNormalSmooth(IMesh* pMesh)
{
    Mesh* mesh = dynamic_cast<Mesh*>(pMesh);
    std::vector<VertexNormalUv>& vertices = mesh->u_pVertexBuffer->vertices;
    std::vector<unsigned short>& indices = mesh->u_pIndexBuffer->indices;

    //for (auto it = smoothData.begin(); it != smoothData.end(); ++it)
    //{
    //    std::vector<unsigned short>& v = it->second;
    //    Vector3 normal;
    //    for (unsigned short index : v)
    //    {
    //        normal += vertices[index].normal;
    //    }
    //    normal.Normalize();
    //    for (unsigned short index : v)
    //    {
    //        vertices[index].normal = normal;
    //    }
    //}
}

void gamelib::MeshFactory::ConvertVertexNormalMap(IMesh* pMesh)
{

}

gamelib::IMesh* gamelib::MeshFactory::CreatePlane(float scale, const Vector3& _offset)
{
    std::vector<VertexNormalUv> vertices(4);
    vertices[0] = { Vector3(-1, 0, -1) * scale, Vector3(0, 1, 0), Vector2(0, 1) };
    vertices[1] = { Vector3(+1, 0, -1) * scale, Vector3(0, 1, 0), Vector2(1, 1) };
    vertices[2] = { Vector3(-1, 0, +1) * scale, Vector3(0, 1, 0), Vector2::Zero() };
    vertices[3] = { Vector3(+1, 0, +1) * scale, Vector3(0, 1, 0), Vector2(1, 0) };
    for (auto& x : vertices)
    {
        x.pos += _offset;
    }
    std::vector<unsigned short> indices = { 0, 2, 1, 3 };
    Mesh* mesh = new Mesh;
    mesh->u_pVertexBuffer->vertices.swap(vertices);
    mesh->u_pIndexBuffer->indices.swap(indices);
    //mesh->u_pVertexBuffer->SetPrimitive(MESH_PRIMITIVE::TRIANGLE_STRIP);
    mesh->CreateBuffers();
    return mesh;
}

gamelib::IMesh* gamelib::MeshFactory::CreatePatchPlane(float point, float eage, float uvPoint, const Vector3& _offset)
{
    float uvsize = 1.0f / uvPoint;
    Mesh* mesh = new Mesh();
    std::vector<VertexNormalUv>& vertices = mesh->u_pVertexBuffer->vertices;
    for (int y = 0; y < point + 1; y++)
    {
        for (int x = 0; x < point + 1; x++)
        {
            VertexNormalUv v;
            v.pos = Vector3(eage * x / point, 0.0f, eage * y / point);
            v.uv = uvsize * Vector2(v.pos.x / eage, 1.0f - (v.pos.z / eage));
            v.normal = Vector3::Up();
            vertices.emplace_back(v);
        }
    }
    for (auto& v : vertices)
    {
        v.pos.x -= eage * 0.5f;
        v.pos.z -= eage * 0.5f;
    }
    std::vector<unsigned short>& indices = mesh->u_pIndexBuffer->indices;
    for (int z = 0; z < point; z++)
    {
        for (int x = 0; x < point; x++)
        {
            const unsigned short rows = point + 1;
            unsigned short v0 = x + rows * z;
            unsigned short v1 = x + 1 + rows * z;
            indices.emplace_back(v0 + rows);
            indices.emplace_back(v1 + rows);
            indices.emplace_back(v0);
            indices.emplace_back(v1);
        }
    }
    mesh->CreateBuffers();
    mesh->u_pVertexBuffer->SetPrimitive(MESH_PRIMITIVE::POINT_4_PATCH);
    return mesh;
}

gamelib::IMesh* gamelib::MeshFactory::CreateTriangle(float scale, const Vector3& _offset)
{
    return nullptr;
}

gamelib::IMesh* gamelib::MeshFactory::CreateBox(float scale, const Vector3& _offset)
{
    std::vector<VertexNormalUv> vertices(4);
    vertices[0] = { Vector3(-1, 0, -1) * scale, Vector3(0, 1, 0), Vector2(0, 1) };
    vertices[1] = { Vector3(+1, 0, -1) * scale, Vector3(0, 1, 0), Vector2(1, 1) };
    vertices[2] = { Vector3(-1, 0, +1) * scale, Vector3(0, 1, 0), Vector2::Zero() };
    vertices[3] = { Vector3(+1, 0, +1) * scale, Vector3(0, 1, 0), Vector2(1, 0) };
    return nullptr;
}

gamelib::IMesh* gamelib::MeshFactory::CreateSphere(float scale, const Vector3& _offset)
{
    return nullptr;
}

gamelib::IMesh* gamelib::MeshFactory::CreateCapsule(float scale, const Vector3& _offset)
{
    return nullptr;
}

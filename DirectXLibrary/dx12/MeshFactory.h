#pragma once
#include "IMesh.h"
#include "../math/Vector.h"
namespace gamelib
{
class MeshFactory
{
private:
	MeshFactory() = delete;
	~MeshFactory() = delete;
public:
	static void ComputeNormalFlat(IMesh* pMesh);
	static void ComputeNormalSmooth(IMesh* pMesh);
	static void ConvertVertexNormalMap(IMesh* pMesh);
	static IMesh* CreatePlane(float scale = 1.0f, const Vector3& _offset = { 0,0,0 });
	static IMesh* CreatePatchPlane(float point, float eage, float uvPoint, const Vector3& _offset = { 0,0,0 });
	static IMesh* CreateTriangle(float scale = 1.0f, const Vector3& _offset = { 0,0,0 });
	static IMesh* CreateBox(float scale = 1.0f, const Vector3& _offset = { 0,0,0 });
	static IMesh* CreateSphere(float scale = 1.0f, const Vector3& _offset = { 0,0,0 });
	static IMesh* CreateCapsule(float scale = 1.0f, const Vector3& _offset = { 0,0,0 });
};
} // namespace gamelib


#pragma once
#include "../dx12/VertexBuffer.h"
#include "../pipeline/IPipelineState.h"

namespace gamelib
{
//ƒŒƒC•`‰æ
class DebugRay
{
private:
	int drawCallCoount;

	std::unique_ptr<VertexBuffer<VertexLineSizeColor>> vertexBuffer;

	std::unique_ptr<IPipelineState> pipeline;
public:
	//ƒŒƒC‚Ì‰Šú‰»
	DebugRay();
	void Draw(const Vector3& start, const Vector3& end, float length = 0.2f, const Vector4& color = { 1,1,1,1 });
	void End();
};
} // namespace gamelib
#pragma once
#include "../dx12/VertexBuffer.h"
#include "../pipeline/IPipelineState.h"

namespace gamelib
{
//レイ描画
class DebugRay
{
private:
	int drawCallCoount;

	std::unique_ptr<VertexBuffer<VertexLineSizeColor>> u_pVertexBuffer;

	std::unique_ptr<IPipelineState> u_pPipeline;
public:
	//レイの初期化
	DebugRay();
	void Draw(const Vector3& start, const Vector3& end, float length = 0.2f, const Vector4& color = { 1,1,1,1 });
	void End();
};
} // namespace gamelib
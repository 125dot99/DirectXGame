#pragma once
#include "../dx12/VertexBuffer.h"
#include "../pipeline/IPipelineState.h"

namespace gamelib
{
//���C�`��
class DebugRay
{
private:
	int drawCallCoount;

	std::unique_ptr<VertexBuffer<VertexLineSizeColor>> vertexBuffer;

	std::unique_ptr<IPipelineState> pipeline;
public:
	//���C�̏�����
	DebugRay();
	void Draw(const Vector3& start, const Vector3& end, float length = 0.2f, const Vector4& color = { 1,1,1,1 });
	void End();
};
} // namespace gamelib
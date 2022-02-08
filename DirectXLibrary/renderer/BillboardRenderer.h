#pragma once
#include "../math/Matrix4.h"
#include "../dx12/Texture.h"
#include "../dx12/VertexBuffer.h"
#include "../pipeline/IPipelineState.h"

namespace gamelib
{
class BillboardRenderer
{
private:
	//�`��I�u�W�F�N�g�̐�
	int objectCount;
	//���_�o�b�t�@
	std::unique_ptr<VertexBuffer<VertexPosColorScaleAngle>> vertexBuffer;
	//�p�C�v���C��
	std::unique_ptr<IPipelineState> pipeline;
public:
	BillboardRenderer();
	~BillboardRenderer();
	
	void Begin();

	void Draw(const std::weak_ptr<Texture>& w_p_texture, const Vector3& pos, const Vector2& scale, const float rotate = 0, const Vector4& color = { 1,1,1,1 });

	void End();
};
} // namespace gamelib


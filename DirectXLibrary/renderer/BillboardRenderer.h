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
	std::unique_ptr<VertexBuffer<VertexPosColorScaleAngle>> u_pVertexBuffer;
	//�p�C�v���C��
	std::unique_ptr<IPipelineState> u_pPipeline;
public:
	BillboardRenderer();
	~BillboardRenderer();
	
	void Begin();

	void Draw(const std::weak_ptr<Texture>& w_pTexture, const Vector3& pos, const Vector2& scale, const float rotate = 0, const Vector4& color = { 1,1,1,1 });

	void End();
};
} // namespace gamelib


#pragma once
#include "../dx12/Texture.h"
#include "../math/Vector.h"
#include "../dx12/VertexBuffer.h"
#include "../dx12/ConstBuffer.h"
#include "../pipeline/IPipelineState.h"
#include <memory>

namespace gamelib
{
class SpriteRenderer
{
private:
	//�X�v���C�g�̐�
	int spriteCount;
	//���_�o�b�t�@
	std::unique_ptr<VertexBuffer<VertexSprite>> u_pVertexBuffer;
	//�萔�o�b�t�@
	std::unique_ptr<ConstBuffer> u_pConstBuffer;
	//�X�v���C�g�̃p�C�v���C��
	std::unique_ptr<IPipelineState> u_pPipeline;
	//�X�v���C�g�̕`��
	void Renderer(
		const std::weak_ptr<Texture>& w_pTexture,
		const Vector2& pos, 
		const Vector2& size, 
		float angle, 
		const Vector2& pivot, 
		const Vector4& rect, 
		const Vector4& color);
public:
	SpriteRenderer();

	/// <summary>
	/// �X�v���C�g�̕`��O����
	/// </summary>
	void Begin();

	void SetScreenSize(const Vector2& screenSize);

	//�ʏ�
	void Draw(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector4& color = { 1,1,1,1 });
	void Draw(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& pivot, const Vector4& color = { 1,1,1,1 });
	
	//���E���]
	void DrawTurnX(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector4& color = { 1,1,1,1 });
	
	//�㉺���]
	void DrawTurnY(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector4& color = { 1,1,1,1 });

	//�T�C�Y�w��
	void DrawExtend(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& size, const Vector4& color = { 1,1,1,1 });
	void DrawExtend(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& size, const Vector2& pivot, const Vector4& color = { 1,1,1,1 });

	//��]
	void DrawRotate(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, float angle, const Vector4& color = { 1,1,1,1 });
	void DrawRotate(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, float angle, const Vector2& pivot, const Vector4& color = { 1,1,1,1 });
	
	//�w�肵����`�����̂ݕ`��
	void DrawRect(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector4& rect, const Vector4& color = { 1,1,1,1 });
	
	//�g��k���A�w�肵����`�����̂ݕ`��
	void DrawExtendRect(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& size, const Vector4& rect, const Vector4& color = { 1,1,1,1 });
	void DrawExtendRect(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& size, const Vector4& rect, const Vector2& pivot, const Vector4& color = { 1,1,1,1 });
	
	void DrawExtendMakeRect(const std::weak_ptr<Texture>& w_pTexture, const Vector2& pos, const Vector2& size, const Vector2& indexSize, int index, const Vector4& color = { 1,1,1,1 });

	/// <summary>
	/// �X�v���C�g�̕`���n��
	/// </summary>
	void End();
};
} // namespace gamelib


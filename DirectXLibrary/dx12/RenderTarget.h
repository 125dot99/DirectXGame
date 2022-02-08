#pragma once
#include "Texture.h"

namespace gamelib
{
//�����_�[�e�N�X�`���N���X
class RenderTexture : public Texture
{
public:
	friend class RenderTarget;
	RenderTexture(const Vector2& _textureSize, DXGI_FORMAT format);
};

class RenderTarget : public Texture
{
private:
	//RTV�̃f�X�N���v�^�q�[�v
	std::weak_ptr<DescriptorHeap> descriptorHeapRTV;
	//�`���`�R�}���h���M
	void RasterizerStageCommand();
	//�����_�[�^�[�Q�b�g�r���[�R�}���h���M
	void OMSetRenderTarget();
	void OMSetRenderTargetClear();
	//�����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���r���[�R�}���h���M
	void OMSetRenderTargetDepth();
	void OMSetRenderTargetDepthClear();
public:
	RenderTarget(const Vector2& textureSize, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	/// <summary>
	/// �����_�[�^�[�Q�b�g�r���[�̍쐬
	/// </summary>
	/// <param name="pDescHeapRTV"></param>
	void CreateRTV(std::weak_ptr<DescriptorHeap> pDescHeapRTV);

	/// <summary>
	/// �����_�[�^�[�Q�b�g�ɐݒ�
	/// </summary>
	/// <param name="isClear">��ʂ��N���A���邩</param>
	/// <param name="isDepth">�[�x�X�e���V�����Z�b�g���邩</param>
	void RBRenderTarget(bool isClear = false, bool isDepth = false);

	/// <summary>
	/// �s�N�Z���V�F�[�_�[���\�[�X�ɐݒ�
	/// </summary>
	void RBPixelShaderResource();
};
} // namespace gamelib


#pragma once
#include "Texture.h"

namespace gamelib
{
class RenderTarget : public Texture
{
private:
	float clearColors[4];
	//RTV�̃f�X�N���v�^�q�[�v
	std::weak_ptr<DescriptorHeap> w_pDescriptorHeapRTV;
public:
	RenderTarget(const Vector2& textureSize, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, const Vector4& clearColor = {});

	/// <summary>
	/// �����_�[�^�[�Q�b�g�r���[�̍쐬
	/// </summary>
	/// <param name="pDescHeapRTV"></param>
	void CreateRTV(const std::weak_ptr<DescriptorHeap>& w_pDescriptorHeapRTV);

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


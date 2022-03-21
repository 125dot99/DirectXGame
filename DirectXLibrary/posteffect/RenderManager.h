#pragma once
#include "../dx12/RenderTarget.h"
#include "../dx12/VertexBuffer.h"
#include <vector>

namespace gamelib
{
class RenderManager
{
private:
	std::unique_ptr<VertexBuffer<VertexUv>> u_pVertexBuffer;
	std::shared_ptr<DescriptorHeap> s_pDescriptorHeapSRV;
	std::shared_ptr<DescriptorHeap> s_pDescriptorHeapRTV;

	//�����_�[�e�N�X�`���̊Ǘ�
	std::vector<std::shared_ptr<RenderTarget>> vecRenderTextures;

	RenderManager();
	~RenderManager();
	RenderManager(const RenderManager&) = delete;
	void operator=(const RenderManager&) = delete;
public:
	/// <summary>
	/// �V���O���g���C���X�^���X
	/// </summary>
	/// <returns></returns>
	static RenderManager* GetInstance();

	/// <summary>
	/// �����_�[�e�N�X�`���̍쐬�A�ǂݎ��p�e�N�X�`���̒ǉ�
	/// </summary>
	/// <param name="postEffect"></param>
	void Add(std::shared_ptr<RenderTarget> s_pRenderTarget);

	/// <summary>
	/// �����_�[�e�N�X�`���̍쐬
	/// </summary>
	/// <param name="pRenderTarget"></param>
	/// <param name="index"></param>
	void Create(RenderTarget* pRenderTarget, UINT index);

	/// <summary>
	/// �`��O����
	/// </summary>
	void WriteStart(bool isClear = true, bool isDepth = true);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	void WriteEnd();

	/// <summary>
	/// �ŏI�I�ȕ`�挋��
	/// </summary>
	void Result();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �폜
	/// </summary>
	void Clear();

	/// <summary>
	/// �e�N�X�`�����擾
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	std::weak_ptr<Texture> GetRenderTarget(int index) const;
};
} // namespace gamelib


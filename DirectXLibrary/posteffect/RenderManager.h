#pragma once
#include "../dx12/RenderTarget.h"
#include "../dx12/VertexBuffer.h"
#include <vector>

namespace gamelib
{
class RenderManager
{
private:
	std::unique_ptr<RenderTarget> sourceTexture;
	std::unique_ptr<VertexBuffer<VertexUv>> vertexBuffer;
	std::shared_ptr<DescriptorHeap> descriptorHeapSRV;
	std::shared_ptr<DescriptorHeap> descriptorHeapRTV;

	//�����_�[�e�N�X�`���̊Ǘ�
	std::vector<RenderTarget*> textures;

	RenderManager();
	~RenderManager() = default;
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
	void Add(RenderTarget* pRenderTarget);

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
	Texture* GetRenderTarget(int index) const;
};
} // namespace gamelib


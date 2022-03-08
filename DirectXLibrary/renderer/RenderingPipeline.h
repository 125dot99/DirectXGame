#pragma once
#include <vector>
#include <memory>
#include <unordered_map>

#include "../pipeline/IPipelineState.h"
#include "../renderer/MeshRenderer.h"
#include "../dx12/RenderTarget.h"

namespace gamelib
{
//�S�Ă̕`��V�X�e����g����
class RenderingPipeline
{
private:
	//�V�F�[�_�[
	std::unordered_map<UINT, std::weak_ptr<IPipelineState>> u_mapPipelines;
	//�`��I�u�W�F�N�g
	std::unordered_map<UINT, std::vector<std::shared_ptr<MeshRenderer>>> u_mapRenderings;
public:
	RenderingPipeline();
	
	/// <summary>
	/// �p�C�v���C����ݒ肷��
	/// </summary>
	/// <param name="index"></param>
	/// <param name="shaderName"></param>
	void SetPipeline(UINT index, const std::string& shaderName);

	/// <summary>
	/// �`��I�u�W�F�N�g��ǉ�
	/// </summary>
	/// <param name="index"></param>
	/// <param name="renderer"></param>
	void Add(UINT index, std::shared_ptr<MeshRenderer> renderer);

	/// <summary>
	/// �`��O����
	/// </summary>
	void Begin();
	
	/// <summary>
	/// �S�ĕ`��
	/// </summary>
	void Draw();
};
} // namespace gamelib

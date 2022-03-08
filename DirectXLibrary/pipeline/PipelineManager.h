#pragma once
#include <unordered_map>
#include <memory>

#include "IPipelineState.h"

namespace gamelib
{
//�O���t�B�b�N�X�p�C�v���C���Ǘ�
class PipelineManager
{
private:
	//�O���t�B�b�N�X�p�C�v���C�����i�[����z��
	std::unordered_map<std::string, std::unique_ptr<IPipelineState>> psoDatas;
	
	PipelineManager() = default;
	~PipelineManager() = default;
	PipelineManager(const PipelineManager&) = delete;
	void operator= (const PipelineManager&) = delete;
public:
	static PipelineManager* GetInstance();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="_pipelineTag"></param>
	/// <returns></returns>
	IPipelineState* GetPipelineState(const std::string& _pipelineTag) const;
};
} // namespace gamelib
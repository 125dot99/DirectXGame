#pragma once
#include <unordered_map>
#include <memory>

#include "IPipelineState.h"

namespace gamelib
{
//グラフィックスパイプライン管理
class PipelineManager
{
private:
	//グラフィックスパイプラインを格納する配列
	std::unordered_map<std::string, std::unique_ptr<IPipelineState>> psoDatas;
	
	PipelineManager() = default;
	~PipelineManager() = default;
	PipelineManager(const PipelineManager&) = delete;
	void operator= (const PipelineManager&) = delete;
public:
	static PipelineManager* GetInstance();

	/// <summary>
	/// 初期化
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

#pragma once
#include <string>

namespace gamelib
{
//パイプライン基底クラス
class IPipelineState
{
public:
	virtual ~IPipelineState() = default;

	virtual void Command() = 0;
};

struct ShaderData
{
	std::string file; //ファイル名
	std::string mainEntry; //関数名
};

enum class ROOT_PARAMETER
{
	WORLD,
	CAMERA,
	LIGHT,
	MATERIAL,
	BONE,
	TEXTURE = -1,//テクスチャだけ差別化
	CBV = WORLD,
	UAV = -2,
};
} // namespace gamelib
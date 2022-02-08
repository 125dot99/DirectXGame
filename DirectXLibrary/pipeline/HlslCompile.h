#pragma once
#include "IPipelineState.h"
#include <d3d12.h>
#include <string>

namespace gamelib
{
namespace hlsl_compile
{
//Hlslのコンパイル
D3D12_SHADER_BYTECODE CompileHlsl(const ShaderData& _hlsl, const std::string& _shaderModel);
//コンパイルエラー内容を出力
void OutputError(ID3DBlob* pErrorblob);

} // namespace hlsl_compil
} // namespace gamelib
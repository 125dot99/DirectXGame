#pragma once
#include "IPipelineState.h"
#include <d3d12.h>
#include <string>

namespace gamelib
{
namespace hlsl_compile
{
//Hlsl�̃R���p�C��
D3D12_SHADER_BYTECODE CompileHlsl(const ShaderData& _hlsl, const std::string& _shaderModel);
//�R���p�C���G���[���e���o��
void OutputError(ID3DBlob* pErrorblob);

} // namespace hlsl_compil
} // namespace gamelib
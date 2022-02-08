#include "HlslCompile.h"

#include <d3dcompiler.h>
#include <d3dx12.h>

#include "../loader/UtilityString.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")

D3D12_SHADER_BYTECODE gamelib::hlsl_compile::CompileHlsl(const ShaderData& _hlsl, const std::string& _shaderModel)
{
	if (_hlsl.file.size() == 0)
	{
		return  D3D12_SHADER_BYTECODE();//�����L�q���Ă��Ȃ���Ζ߂�
	}
	ID3DBlob* shaderBlob, * errorBlob;
	//���C�u�����Ɉˑ�����V�F�[�_
	const std::string SHADER_PATH = "HLSL/";
	auto wfilePath = utility_string::StrToWchar(SHADER_PATH + _hlsl.file);
	HRESULT result = D3DCompileFromFile(
		wfilePath,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_hlsl.mainEntry.c_str(),
		_shaderModel.c_str(),
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&shaderBlob, &errorBlob);
	if (FAILED(result))
	{
		OutputError(errorBlob);
	}
	return CD3DX12_SHADER_BYTECODE(shaderBlob);
}

void gamelib::hlsl_compile::OutputError(ID3DBlob* pErrorblob)
{
	//errorBlob����G���[���e��string�^�ɃR�s�[
	std::string errstr;
	errstr.resize(pErrorblob->GetBufferSize());
	std::copy_n((char*)pErrorblob->GetBufferPointer(), pErrorblob->GetBufferSize(), errstr.begin());
	errstr += "\n";
	OutputDebugStringA(errstr.c_str());//�G���[���e���o�̓E�B���h�E�ɕ\��
	exit(1);
}

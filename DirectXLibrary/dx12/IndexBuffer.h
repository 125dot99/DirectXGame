#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>

#include "Dx12Renderer.h"

namespace gamelib
{
class IndexBuffer
{
private:
	//�C���f�b�N�X�̗v�f��
	UINT indexSize;
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW iBView{};
	//�C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
public:
	//�C���f�b�N�X�z��
	std::vector<unsigned short> indices;

	/// <summary>
	/// �o�b�t�@�𐶐�����
	/// </summary>
	void Create();
	
	/// <summary>
	/// �o�b�t�@�}�b�s���O����
	/// </summary>
	void Map();
	
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@��GPU�R�}���h
	/// </summary>
	inline void Draw()
	{
		static auto* cmdList = Dx12Renderer::GetCommandList();
		cmdList->IASetIndexBuffer(&iBView);
		cmdList->DrawIndexedInstanced(indexSize, 1, 0, 0, 0);
	}
};
} // namespace gamelib

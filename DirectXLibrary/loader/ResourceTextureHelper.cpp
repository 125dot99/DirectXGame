#include "ResourceTextureHelper.h"

#include <DirectXTex.h>
#include <d3dx12.h>
#include <assert.h>
#include <fstream>
#include <wincodec.h>

#include "../dx12/Dx12Renderer.h"
#include "../dx12/GpuCopyPixels.h"
#include "UtilityString.h"

ID3D12Resource* gamelib::ResourceTextureHelper::LoadTexturePng(const std::string& fileName)
{
	return nullptr;
}

#pragma pack(1)
struct BitmapHeader
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
};
#pragma pack()

ID3D12Resource* gamelib::ResourceTextureHelper::LoadTextureBmp(const std::string& fileName)
{
	std::ifstream file(fileName, std::ios::binary);
	assert(file.is_open() && "読み取りに失敗しました");

	BitmapHeader bmpHeader;
	file.read((char*)&bmpHeader, sizeof(BitmapHeader));

	auto w = bmpHeader.infoHeader.biWidth;
	auto h = bmpHeader.infoHeader.biHeight;

	int pads[] = { 0, 3, 2, 1 };
	int step = 3 * w;
	step += pads[step % 4];

	std::vector<unsigned char> pixels(step * h);
	file.read((char*)pixels.data(), pixels.size());
	std::vector<unsigned char> imagePixels(3 * w * h);
	for (int i = 0; i < h; i++)
	{
		UINT64 index = static_cast<UINT64>(i);
		memcpy(imagePixels.data() + index * 3 * w, pixels.data() + (h - index - 1) * step, 3 * w);
	}
	for (int i = 0, end = (int)imagePixels.size() / 3; i < end; i++)
	{
		UINT64 index = static_cast<UINT64>(i);
		unsigned char pixel = imagePixels[index * 3 + 2];
		imagePixels[index * 3 + 2] = imagePixels[index * 3 + 0];
		imagePixels[index * 3 + 0] = pixel;
	}
	return nullptr;
}

ID3D12Resource* gamelib::ResourceTextureHelper::CreateTextureBuffer(const D3D12_RESOURCE_DESC& resourceDesc)
{
	return nullptr;
}

ID3D12Resource* gamelib::ResourceTextureHelper::ReadTextureBuffer(const std::string& fileName)
{
	DirectX::ScratchImage scratchImg;
	DirectX::TexMetadata meta;
	HRESULT result = DirectX::LoadFromWICFile(
		utility_string::StrToWchar(fileName),
		DirectX::WIC_FLAGS_NONE,
		&meta,
		scratchImg);
	assert(SUCCEEDED(result) && "テクスチャ読み込み失敗");
	
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		meta.format,
		static_cast<UINT64>(meta.width),
		static_cast<UINT>(meta.height),
		static_cast<UINT16>(meta.arraySize),
		static_cast<UINT16>(meta.mipLevels));
	
	//バッファ生成
	ID3D12Resource* textureBuffer;
	result = Dx12Renderer::GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textureBuffer));
	assert(SUCCEEDED(result) && "テクスチャバッファ生成失敗");

	//テクスチャバッファにデータ転送
	const DirectX::Image* img = scratchImg.GetImage(0, 0, 0);
	result = textureBuffer->WriteToSubresource(
		0,
		nullptr, //全領域へコピー
		img->pixels, //元データアドレス
		static_cast<UINT>(img->rowPitch), //1ラインサイズ
		static_cast<UINT>(img->slicePitch)); //全サイズ
	assert(SUCCEEDED(result) && "テクスチャバッファ転送失敗");
	return textureBuffer;
}

void gamelib::ResourceTextureHelper::SaveTextureBuffer(const std::string& fileName, const MiniImage& image)
{
	//出力先
	const std::string PATH = "Resources/ScreenShot/";
	size_t width = static_cast<size_t>(image.size.x);
	size_t height = static_cast<size_t>(image.size.y);
	DirectX::ScratchImage scratchImg;
	scratchImg.Initialize2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 0);
	size_t rowPitch;
	size_t slicePitch;
	DirectX::ComputePitch(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, rowPitch, slicePitch);
	uint8_t* p = scratchImg.GetPixels();
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			size_t offset = rowPitch * y + x * 4;
			size_t index = x + y * width;
			printf("%zd, ", index);
			p[offset + 0] = static_cast<uint8_t>(image.pixels[index].x);
			p[offset + 1] = static_cast<uint8_t>(image.pixels[index].y);
			p[offset + 2] = static_cast<uint8_t>(image.pixels[index].z);
			p[offset + 3] = static_cast<uint8_t>(image.pixels[index].w);
		}
	}
	HRESULT result = DirectX::SaveToWICFile(
		*scratchImg.GetImages(),
		DirectX::WIC_FLAGS_NONE,
		GUID_ContainerFormatPng, //GetWICCodec(DirectX::WIC_CODEC_PNG),
		utility_string::StrToWchar(PATH + fileName));
	assert(SUCCEEDED(result) && "スクリーンショットの保存失敗");
}

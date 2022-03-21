#pragma once
#include "Texture.h"
#include "DescriptorHeap.h"
#include "ConstBuffer.h"
#include "RWStructuredBuffer.h"
#include "../pipeline/IPipelineState.h"
#include "../math/Vector.h"
#include <vector>

//コンピュートシェーダを使ってピクセルを取得
namespace gamelib
{
struct MiniImage
{
	std::vector<Vector4> pixels;
	Vector2 size;
};

class GpuCopyPixels
{
private:
	std::unique_ptr<MiniImage> imageData;
	std::shared_ptr<DescriptorHeap> descHeap;
	std::unique_ptr<ConstBuffer> u_pConstBuffer;
	std::unique_ptr<RWStructuredBuffer> u_pUavBuffer;
	std::weak_ptr<IPipelineState> w_pPipeline;
public:
	GpuCopyPixels();

	MiniImage Dispatch(std::weak_ptr<Texture> w_p_sourceTexture);
};
} // gamelib

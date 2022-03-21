#include "GpuCopyPixels.h"

#include "../posteffect/RenderManager.h"
#include "../pipeline/PipelineManager.h"
#include "../app/Application.h"

gamelib::GpuCopyPixels::GpuCopyPixels()
{
	u_pConstBuffer = std::make_unique<ConstBuffer>();
	descHeap = std::make_shared<DescriptorHeap>();
	descHeap->Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);
	u_pUavBuffer = std::make_unique<RWStructuredBuffer>();

	imageData = std::make_unique<MiniImage>();
	imageData->size = Application::GetInstance()->GetWindowSize();
	size_t bufferSize = static_cast<size_t>(imageData->size.x) * static_cast<size_t>(imageData->size.y);
	imageData->pixels.resize(bufferSize);
	u_pUavBuffer->CreateUAV(sizeof(Vector4) * bufferSize, descHeap, 0);
	w_pPipeline = PipelineManager::GetInstance()->GetPipelineState("SaveTextureCS");
}

gamelib::MiniImage gamelib::GpuCopyPixels::Dispatch(std::weak_ptr<Texture> w_p_sourceTexture)
{
	w_pPipeline.lock()->Command();
	w_p_sourceTexture.lock()->ComputeSRVCommand(0);
	u_pUavBuffer->ComputeUAVCommand(1);
	u_pUavBuffer->Dispatch((UINT)imageData->size.x, (UINT)imageData->size.y, 1);
	int size = (int)imageData->pixels.size();
	memcpy(imageData->pixels.data(), (Vector4*)u_pUavBuffer->GetCSOutput(), size);
	//for (int i = 0; i < size; i++)
	//{
	//	Vector4 v = imageData->pixels[i];
	//	if ((v.x + v.y + v.z + v.w) == 0)
	//	{
	//		continue;
	//	}
	//	printf("[%d], %f, %f, %f, %f\n", i, v.x, v.y, v.z, v.w);
	//}
	return *imageData.get();
}

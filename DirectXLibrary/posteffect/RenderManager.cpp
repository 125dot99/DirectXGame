#include "RenderManager.h"

#include "../pipeline/PipelineManager.h"

gamelib::RenderManager::RenderManager()
{
    vertexBuffer = std::make_unique<VertexBuffer<VertexUv>>();
    vertexBuffer->Create(4, MESH_PRIMITIVE::TRIANGLE_STRIP);

    VertexUv vertices[4] = {
        { Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f) },	//右下
        { Vector3(-1.0f, +1.0f, 0.0f), Vector2(0.0f, 0.0f) },	//右上
        { Vector3(+1.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f) },	//左下
        { Vector3(+1.0f, +1.0f, 0.0f), Vector2(1.0f, 0.0f) } }; //左上
    vertexBuffer->Map(vertices);

    descriptorHeapSRV = std::make_shared<DescriptorHeap>();
    descriptorHeapSRV->Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 32);
    
    descriptorHeapRTV = std::make_shared<DescriptorHeap>();
    descriptorHeapRTV->Create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 32);

    //描画元テクスチャ
    sourceTexture = std::make_unique<RenderTarget>(Vector2::Zero());
    Add(sourceTexture.get());
}

gamelib::RenderManager* gamelib::RenderManager::GetInstance()
{
    static RenderManager instance;
    return &instance;
}

void gamelib::RenderManager::Add(RenderTarget* pRenderTarget)
{
    textures.emplace_back(pRenderTarget);
    Create(pRenderTarget, (UINT)textures.size() - 1);
}

void gamelib::RenderManager::Create(RenderTarget* pRenderTarget, UINT index)
{
    pRenderTarget->CreateSRV(descriptorHeapSRV, index);
    pRenderTarget->CreateRTV(descriptorHeapRTV);
}

void gamelib::RenderManager::WriteStart(bool isClear, bool isDepth)
{
    textures[0]->RBRenderTarget(isClear, isDepth);
}

void gamelib::RenderManager::WriteEnd()
{
    textures[0]->RBPixelShaderResource();
}

void gamelib::RenderManager::Result()
{
    PipelineManager::GetInstance()->GetPipelineState("PE_None")->Command();
    textures[0]->GraphicsSRVCommand(0);
    Draw();
}

void gamelib::RenderManager::Draw()
{
    vertexBuffer->BufferCommand();
    vertexBuffer->Draw();
}

void gamelib::RenderManager::Clear()
{
    if (textures.size() == 1)
    {
        return;
    }
    for (int i = 1; i < (int)textures.size(); i++)
    {
        textures.erase(std::remove(textures.begin(), textures.end(), textures[i]));
        i--;
    }
}

gamelib::Texture* gamelib::RenderManager::GetRenderTarget(int index) const
{
    return index >= textures.size() ? textures[0] : textures[index];
}

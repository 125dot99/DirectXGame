#include "RenderManager.h"

#include "../pipeline/PipelineManager.h"

gamelib::RenderManager::RenderManager()
{
    u_pVertexBuffer = std::make_unique<VertexBuffer<VertexUv>>();
    u_pVertexBuffer->Create(4, MESH_PRIMITIVE::TRIANGLE_STRIP);

    VertexUv vertices[4] = {
        { Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f) },	//右下
        { Vector3(-1.0f, +1.0f, 0.0f), Vector2(0.0f, 0.0f) },	//右上
        { Vector3(+1.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f) },	//左下
        { Vector3(+1.0f, +1.0f, 0.0f), Vector2(1.0f, 0.0f) } }; //左上
    u_pVertexBuffer->Map(vertices);

    s_pDescriptorHeapSRV = std::make_shared<DescriptorHeap>();
    s_pDescriptorHeapSRV->Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 32);
    
    s_pDescriptorHeapRTV = std::make_shared<DescriptorHeap>();
    s_pDescriptorHeapRTV->Create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 32);

    //描画元テクスチャ
    Add(std::make_shared<RenderTarget>(Vector2::Zero()));
}

gamelib::RenderManager::~RenderManager()
{

}

gamelib::RenderManager* gamelib::RenderManager::GetInstance()
{
    static RenderManager instance;
    return &instance;
}

void gamelib::RenderManager::Add(std::shared_ptr<RenderTarget> s_pRenderTarget)
{
    vecRenderTextures.emplace_back(s_pRenderTarget);
    s_pRenderTarget->CreateSRV(s_pDescriptorHeapSRV, (UINT)vecRenderTextures.size() - 1);
    s_pRenderTarget->CreateRTV(s_pDescriptorHeapRTV);
}

void gamelib::RenderManager::Create(RenderTarget* pRenderTarget, UINT index)
{
    pRenderTarget->CreateSRV(s_pDescriptorHeapSRV, index);
    pRenderTarget->CreateRTV(s_pDescriptorHeapRTV);
}

void gamelib::RenderManager::WriteStart(bool isClear, bool isDepth)
{
    vecRenderTextures[0]->RBRenderTarget(isClear, isDepth);
}

void gamelib::RenderManager::WriteEnd()
{
    vecRenderTextures[0]->RBPixelShaderResource();
}

void gamelib::RenderManager::Result()
{
    PipelineManager::GetInstance()->GetPipelineState("PE_None").lock()->Command();
    vecRenderTextures[0]->GraphicsSRVCommand(0);
    Draw();
}

void gamelib::RenderManager::Draw()
{
    u_pVertexBuffer->BufferCommand();
    u_pVertexBuffer->Draw();
}

void gamelib::RenderManager::Clear()
{
    for (int i = 1; i < vecRenderTextures.size(); i++)
    {
        vecRenderTextures.erase(std::remove(vecRenderTextures.begin(), vecRenderTextures.end(), vecRenderTextures[i]));
        i--;
    }
}

std::weak_ptr<gamelib::Texture> gamelib::RenderManager::GetRenderTarget(int index) const
{
    return index >= vecRenderTextures.size() ? vecRenderTextures[0] : vecRenderTextures[index];
}

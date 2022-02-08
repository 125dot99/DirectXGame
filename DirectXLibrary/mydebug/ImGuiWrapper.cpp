#include "ImGuiWrapper.h"

#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx12.h"

#include "../dx12/Dx12Renderer.h"
#include "../dx12/DescriptorHeap.h"
#include "../app/Application.h"

gamelib::ImGuiWrapper::ImGuiWrapper()
{
	descriptorHeap = std::make_unique<DescriptorHeap>();
	descriptorHeap->Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);
	//ImGuiの内部のバージョンチェック
	IMGUI_CHECKVERSION();
	//ImGui本体の初期化
	ImGui::CreateContext();
	//windows向けに初期化
	if (!ImGui_ImplWin32_Init(Application::GetInstance()->GetHwnd()))
	{
		assert(!"アプリケーションハンドルが取得できません");
	}
	//DirectX12でレンダリングするための初期化
	auto guiFlag = ImGui_ImplDX12_Init(
		Dx12Renderer::GetDevice(),
		2,							//バックバッファ数
		DXGI_FORMAT_R8G8B8A8_UNORM,	//バックバッファのフォーマット
		(ID3D12DescriptorHeap*)descriptorHeap.get(), //デスクリプタヒープ
		descriptorHeap->GetCPUHandle(0),
		descriptorHeap->GetGPUHandle(0));
	assert(guiFlag && "ImGuiの初期化失敗");
}

gamelib::ImGuiWrapper::~ImGuiWrapper()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gamelib::ImGuiWrapper::PreFrame()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gamelib::ImGuiWrapper::Renderer()
{
	descriptorHeap->Command();
	ImGui::Renderer();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), Dx12Renderer::GetCommandList());
}


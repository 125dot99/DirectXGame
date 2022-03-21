#include "ImGuiWrapper.h"

#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx12.h"

#include "../dx12/Dx12Renderer.h"
#include "../dx12/DescriptorHeap.h"
#include "../app/Application.h"

gamelib::ImGuiWrapper::ImGuiWrapper()
{
	u_pDescriptorHeap = std::make_unique<DescriptorHeap>();
	u_pDescriptorHeap->Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);
	//ImGui�̓����̃o�[�W�����`�F�b�N
	IMGUI_CHECKVERSION();
	//ImGui�{�̂̏�����
	ImGui::CreateContext();
	//windows�����ɏ�����
	if (!ImGui_ImplWin32_Init(Application::GetInstance()->GetHwnd()))
	{
		assert(!"�A�v���P�[�V�����n���h�����擾�ł��܂���");
	}
	//DirectX12�Ń����_�����O���邽�߂̏�����
	auto guiFlag = ImGui_ImplDX12_Init(
		Dx12Renderer::GetDevice(),
		2,							//�o�b�N�o�b�t�@��
		DXGI_FORMAT_R8G8B8A8_UNORM,	//�o�b�N�o�b�t�@�̃t�H�[�}�b�g
		reinterpret_cast<ID3D12DescriptorHeap*>(u_pDescriptorHeap.get()), //�f�X�N���v�^�q�[�v
		u_pDescriptorHeap->GetCPUHandle(0),
		u_pDescriptorHeap->GetGPUHandle(0));
	assert(guiFlag && "ImGui�̏��������s");
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
	u_pDescriptorHeap->Command();
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), Dx12Renderer::GetCommandList());
}


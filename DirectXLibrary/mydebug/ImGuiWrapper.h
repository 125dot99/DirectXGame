#pragma once
#include <memory>
#include <d3d12.h>
#include "../imgui/imgui.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

namespace gamelib
{
class ImGuiWrapper
{
private:
	std::unique_ptr<class DescriptorHeap> descriptorHeap;
public:
	//ImGui����������
	ImGuiWrapper();
	//ImGui�I������
	~ImGuiWrapper();
	//�t���[���J�n����1�x�����ǂݍ���
	void PreFrame();
	//ImGui�̕`��
	void Renderer();
};
} // namespace gamelib

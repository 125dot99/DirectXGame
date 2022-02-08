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
	//ImGui初期化処理
	ImGuiWrapper();
	//ImGui終了処理
	~ImGuiWrapper();
	//フレーム開始時に1度だけ読み込む
	void PreFrame();
	//ImGuiの描画
	void Renderer();
};
} // namespace gamelib

#include "Application.h"

#include <assert.h>
#include "Fps.h"
#include "../mydebug/ImGuiWrapper.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//imguiの操作を処理
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return 1;
	}
	//メッセージで分岐
	switch (msg)
	{
	case WM_DESTROY://ウィンドウが破棄された
		PostQuitMessage(0);//OSに対して、アプリの終了を伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//標準の処理を行う
}

gamelib::Application::~Application()
{
	//ウィンドウ解除
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

void gamelib::Application::CreateGameWindow()
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = (WNDPROC)WindowProc;
	windowClass.lpszClassName = (LPCWSTR)windowName;
	windowClass.hInstance = GetModuleHandle(nullptr);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClassEx(&windowClass);
	unsigned int windowStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;	//標準設定
	if (isFullScreen)
	{
		windowSize.x = (float)GetSystemMetrics(SM_CXSCREEN);
		windowSize.y = (float)GetSystemMetrics(SM_CYSCREEN);
		windowStyle = WS_POPUP;//最大化
	}
	RECT wrc = { 0, 0, (LONG)windowSize.x, (LONG)windowSize.y };
	AdjustWindowRect(&wrc, windowStyle, false);
	//ウィンドウの位置を中心にする
	int x = (GetSystemMetrics(SM_CXSCREEN) - (int)windowSize.x) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - (int)windowSize.y) / 2;
	hwnd = CreateWindow(
		windowClass.lpszClassName,
		windowClass.lpszClassName,
		windowStyle,
		x,
		y,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		windowClass.hInstance,
		nullptr);
	fps = std::make_unique<Fps>();
	assert(hwnd && "ウィンドウ生成失敗");
}

gamelib::Application* gamelib::Application::GetInstance()
{
	static Application instance;
	return &instance;
}

void gamelib::Application::SetWindow(float width, float height, const LPCWSTR& _name, bool fullScreen)
{
	windowSize = Vector2(width, height);
	windowName = _name;
	isFullScreen = fullScreen;
}

void gamelib::Application::Initialize()
{
	CreateGameWindow();
	ShowWindow(hwnd, isFullScreen ? SW_MAXIMIZE : SW_SHOW);//フルスクリーンか、現在のサイズと位置
}

int gamelib::Application::Run()
{	
	fps->Update();
	MSG msg{};
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msg.message == WM_QUIT)
	{
		return 0;
	}
	return 1;
}


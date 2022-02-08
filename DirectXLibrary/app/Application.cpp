#include "Application.h"

#include <assert.h>
#include "Fps.h"
#include "../mydebug/ImGuiWrapper.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//imgui�̑��������
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return 1;
	}
	//���b�Z�[�W�ŕ���
	switch (msg)
	{
	case WM_DESTROY://�E�B���h�E���j�����ꂽ
		PostQuitMessage(0);//OS�ɑ΂��āA�A�v���̏I����`����
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);//�W���̏������s��
}

gamelib::Application::~Application()
{
	//�E�B���h�E����
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
	unsigned int windowStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;	//�W���ݒ�
	if (isFullScreen)
	{
		windowSize.x = (float)GetSystemMetrics(SM_CXSCREEN);
		windowSize.y = (float)GetSystemMetrics(SM_CYSCREEN);
		windowStyle = WS_POPUP;//�ő剻
	}
	RECT wrc = { 0, 0, (LONG)windowSize.x, (LONG)windowSize.y };
	AdjustWindowRect(&wrc, windowStyle, false);
	//�E�B���h�E�̈ʒu�𒆐S�ɂ���
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
	assert(hwnd && "�E�B���h�E�������s");
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
	ShowWindow(hwnd, isFullScreen ? SW_MAXIMIZE : SW_SHOW);//�t���X�N���[�����A���݂̃T�C�Y�ƈʒu
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


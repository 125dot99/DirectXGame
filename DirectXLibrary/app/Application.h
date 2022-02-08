#pragma once
#include <Windows.h>
#include <memory>
#include "../math/Vector.h"

namespace gamelib
{
//WindowsAPI���܂Ƃ߂��N���X
class Application
{
private:
	bool isFullScreen;
	//�E�B���h�E��
	Vector2 windowSize;
	//�E�B���h�E��
	LPCWSTR windowName = L"DirectX";
	//�E�B���h�E�̐ݒ���s��
	WNDCLASSEXW windowClass{};
	//�E�B���h�E�n���h��
	HWND hwnd{};
	//FPS�Œ�p
	std::unique_ptr<class Fps> fps;
	Application() = default;
	~Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

	//�E�B���h�E����
	void CreateGameWindow();
public:
	//�V���O���g���C���X�^���X
	static Application* GetInstance();
	//�E�B���h�E�T�C�Y�A�E�B���h�E�^�C�g���A�t���X�N���[���ɂ��邩�ǂ�����ݒ�
	void SetWindow(float width, float height, const LPCWSTR& _name, bool isFullScreen);
	//������
	void Initialize();
	//�A�v���P�[�V�����̃��[�v
	int Run();
	//�E�B���h�E����
	inline const Vector2& GetWindowSize() const { return windowSize; }
	//�E�B���h�E�n���h��
	inline const HWND& GetHwnd() const { return hwnd; };
};
} // namespace gamelib


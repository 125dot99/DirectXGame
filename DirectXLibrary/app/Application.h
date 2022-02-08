#pragma once
#include <Windows.h>
#include <memory>
#include "../math/Vector.h"

namespace gamelib
{
//WindowsAPIをまとめたクラス
class Application
{
private:
	bool isFullScreen;
	//ウィンドウ幅
	Vector2 windowSize;
	//ウィンドウ名
	LPCWSTR windowName = L"DirectX";
	//ウィンドウの設定を行う
	WNDCLASSEXW windowClass{};
	//ウィンドウハンドル
	HWND hwnd{};
	//FPS固定用
	std::unique_ptr<class Fps> fps;
	Application() = default;
	~Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

	//ウィンドウ生成
	void CreateGameWindow();
public:
	//シングルトンインスタンス
	static Application* GetInstance();
	//ウィンドウサイズ、ウィンドウタイトル、フルスクリーンにするかどうかを設定
	void SetWindow(float width, float height, const LPCWSTR& _name, bool isFullScreen);
	//初期化
	void Initialize();
	//アプリケーションのループ
	int Run();
	//ウィンドウ横幅
	inline const Vector2& GetWindowSize() const { return windowSize; }
	//ウィンドウハンドル
	inline const HWND& GetHwnd() const { return hwnd; };
};
} // namespace gamelib


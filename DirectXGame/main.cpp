#include <GameLibHeader.h>
#include "dx12/Dx12Renderer.h"
#include "scene/SceneManager.h"
#include "TitleScene.h"
#include "CafeScene.h"
#include "ResourceLoader.h"
#include <memory>

using namespace gamelib;

#ifdef _DEBUG
int main()
#else
//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
#endif // DEBUG
{
	//ウィンドウサイズ
	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;
	//ウィンドウ生成
	auto app = Application::GetInstance();
	app->SetWindow(WINDOW_WIDTH, WINDOW_HEIGHT, L"アンビット Ambit", true);
	app->Initialize();
	//DirectX12デバイス生成
	auto dx12Renderer = std::make_unique<Dx12Renderer>();
	dx12Renderer->Initialize();
	//リソース読み込み
	resources_loader::Load();
	//GUI
	auto debugGui = std::make_unique<ImGuiWrapper>();
	//入力デバイス初期化
	Input::Initialize();
	auto renderManager = RenderManager::GetInstance();
	//シーンマネージャー
	auto sceneManager = std::make_unique<SceneManager>(new TitleScene);
	while (app->Run())//ゲームループ
	{
		//更新
		debugGui->PreFrame();
		Input::Update();
		sceneManager->Update();
		//描画準備
		renderManager->WriteStart();
		//描画
		sceneManager->Draw();
		renderManager->WriteEnd();
		dx12Renderer->DrawBegin();
		renderManager->Result();
		debugGui->Renderer();
		dx12Renderer->DrawEnd();
		if (Input::GetKeyborad()->IsKey(KEY_CODE::ESCAPE))
		{
			break;
		}
	}
	return 0;
}
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
//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
#endif // DEBUG
{
	//�E�B���h�E�T�C�Y
	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;
	//�E�B���h�E����
	auto app = Application::GetInstance();
	app->SetWindow(WINDOW_WIDTH, WINDOW_HEIGHT, L"�A���r�b�g Ambit", true);
	app->Initialize();
	//DirectX12�f�o�C�X����
	auto dx12Renderer = std::make_unique<Dx12Renderer>();
	dx12Renderer->Initialize();
	//���\�[�X�ǂݍ���
	resources_loader::Load();
	//GUI
	auto debugGui = std::make_unique<ImGuiWrapper>();
	//���̓f�o�C�X������
	Input::Initialize();
	auto renderManager = RenderManager::GetInstance();
	//�V�[���}�l�[�W���[
	auto sceneManager = std::make_unique<SceneManager>(new TitleScene);
	while (app->Run())//�Q�[�����[�v
	{
		//�X�V
		debugGui->PreFrame();
		Input::Update();
		sceneManager->Update();
		//�`�揀��
		renderManager->WriteStart();
		//�`��
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
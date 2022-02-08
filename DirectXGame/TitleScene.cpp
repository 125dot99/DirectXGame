#include "TitleScene.h"

#include "GameScene1.h"
#include "SceneFade.h"
#include "GameClock.h"

enum TEXTURE
{
	TITLE,
};

void TitleScene::Initialize()
{
	textures.resize(1);
	textures[TITLE] = ResourceManager::GetInstance()->GetTexture("title.png");
	spriteRenderer = Factory::CreateUnique<SpriteRenderer>();
	fadeScene = Factory::CreateUnique<SceneFadeOut>(spriteRenderer.get());
}

void TitleScene::Update()
{
	fadeScene->Update();
	if (fadeScene->IsNext())
	{
		if (fadeScene->IsState("FadeOut"))
		{
			fadeScene = Factory::CreateUnique<BaseChildScene>();
		}
		else if (fadeScene->IsState("FadeIn"))
		{
			nextSceneState = true;
		}
	}
	if (!fadeScene->IsState("None"))
	{
		return;
	}
	if (Input::GetKeyborad()->IsKeyDown(KEY_CODE::SPACE) ||
		Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::B))
	{
		nextSceneState = true;
	}
}

void TitleScene::Draw()
{
	Vector2 size= Application::GetInstance()->GetWindowSize();
	spriteRenderer->Begin();
	spriteRenderer->Draw(textures[TITLE], size / 2 + Vector2(0, 100), Vector2(0.5f, 1), Vector4(1.0f));
	fadeScene->Draw();
	spriteRenderer->End();
}

void TitleScene::Finalize()
{
	GameClock::Initialize();
}

void TitleScene::NextScene(ISceneManager* pSceneManager)
{
	pSceneManager->ChangeScene(new GameScene1);
}


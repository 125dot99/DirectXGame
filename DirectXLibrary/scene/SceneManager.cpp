#include "SceneManager.h"

gamelib::SceneManager::SceneManager(BaseScene* pMainRootScene)
{
	scene.reset(pMainRootScene);
	scene->Initialize();
}

gamelib::SceneManager::~SceneManager()
{
	scene->Finalize();
}

void gamelib::SceneManager::Next()
{
	scene->NextScene(this);
}

void gamelib::SceneManager::ResetScene()
{
	scene->Finalize();
	scene->Initialize();
}

void gamelib::SceneManager::Update()
{
	scene->Update();
	if (scene->GetNextScene())
	{
		Next();
	}
}

void gamelib::SceneManager::Draw()
{
	scene->Draw();
}

void gamelib::SceneManager::ChangeScene(BaseScene* newScene)
{
	scene->Finalize();
	scene.reset(newScene);
	scene->Initialize();
	scene->Update();
}

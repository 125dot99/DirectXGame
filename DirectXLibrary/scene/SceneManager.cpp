#include "SceneManager.h"

gamelib::SceneManager::SceneManager(BaseScene* pMainRootScene)
{
	u_pScene.reset(pMainRootScene);
	u_pScene->Initialize();
}

gamelib::SceneManager::~SceneManager()
{
	u_pScene->Finalize();
}

void gamelib::SceneManager::Next()
{
	u_pScene->NextScene(this);
}

void gamelib::SceneManager::ResetScene()
{
	u_pScene->Finalize();
	u_pScene->Initialize();
}

void gamelib::SceneManager::Update()
{
	u_pScene->Update();
	if (u_pScene->GetNextScene())
	{
		Next();
	}
}

void gamelib::SceneManager::Draw()
{
	u_pScene->Draw();
}

void gamelib::SceneManager::ChangeScene(BaseScene* newScene)
{
	u_pScene->Finalize();
	u_pScene.reset(newScene);
	u_pScene->Initialize();
	u_pScene->Update();
}

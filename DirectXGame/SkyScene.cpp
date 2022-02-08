#include "SkyScene.h"

#include "ObjectInfo.h"
#include "SafeDelete.h"

void scene::SkyScene::Initialize()
{
	camera = new Camera();
	camera->Initialize(60.0f, 0.01f, 100.0f);

	light = new DirectionalLight();
	light->Initialize(Vector3(-1.0f, 1.0f, 0.0f));

	ObjectInfo::SetMainCamera(camera);
	ObjectInfo::SetMainLight(light);

	skyObject = new SkyFromAtmosphere();
	skyObject->CreateObjModel("sphere");
	skyObject->Initialize();
}

void scene::SkyScene::Update()
{
	camera->Update();

	light->Update();

	skyObject->Update();
}

void scene::SkyScene::Draw()
{
	skyObject->Draw();
}

void scene::SkyScene::Finalize()
{
	Release(camera);
	Release(light);
	Release(skyObject);
}

void scene::SkyScene::NextScene(SceneManager* pScene)
{

}

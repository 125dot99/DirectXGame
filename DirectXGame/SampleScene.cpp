#include "SampleScene.h"

#include "loader/JsonLevelLoader.h"

void SampleScene::Initialize()
{
	//カメラの初期化
	mainCamera = Factory::CreateUnique<Camera>();
	mainCamera->Initialize(60.0f, 0.1f, 100.0f);
	auto cameraTrans = mainCamera->GetTransform();
	cameraTrans->eye = Vector3(0, 1, -5);
	cameraTrans->target = Vector3::Zero();
	mainCamera->ChangeCamera(Factory::Create<CameraDebugMode>(cameraTrans));

	lightCamera = Factory::CreateUnique<Camera>(*mainCamera.get());
	lightCamera->GetTransform()->up = Vector3::Right();

	//ライトの初期化
	lightGroup = Factory::CreateUnique<LightGroup>();
	light.direction = Vector3(-1, 1, 1).Normalize();
	light.color = Vector3::One();
	lightGroup->SetAmbientLight(Vector3::One());
	lightGroup->SetDirectionalLight(&light);

	//shader = PipelineManager::GetInstance()->GetPipelineState("ToonShader").lock().get();
	//描画クラス
	meshRenderer = Factory::CreateUnique<MeshRenderer>();
}

void SampleScene::Update()
{
	mainCamera->Update();
	lightGroup->Update();
	objectManager->Update();
}

void SampleScene::Draw()
{
	shader->Command();
	mainCamera->RegisterCommand();
	lightGroup->RegisterAll();
	for (int i = 0, end = (int)objects.size(); i < end; i++)
	{
		//meshRenderer->Draw(*objects[i], models[i]);
	}
}

void SampleScene::Finalize()
{

}

void SampleScene::NextScene(ISceneManager* pSceneManager)
{

}

#include "SampleScene.h"

#include "loader/JsonLevelLoader.h"

void SampleScene::Initialize()
{
	auto levelData = JsonLevelLoader::LoadFile("test");
	//リソースの取得
	auto resourceManager = ResourceManager::GetInstance();
	auto size = levelData->objects.size();
	models.resize(size);
	objects.resize(size);
	objectManager = Factory::CreateUnique<ObjectManager>();
	for (int i = 0; i < (int)size; i++)
	{
		//データ読み込み
		Vector3 pos = levelData->objects[i].translation;
		Vector3 rot = levelData->objects[i].rotation;
		Vector3 scale = levelData->objects[i].scaling;

		models[i] = resourceManager->GetModel(levelData->objects[i].fileName);
		objects[i] = Factory::Create<GameObject>(pos, rot, scale);
		objectManager->Add(objects[i]);
	}

	//カメラの初期化
	mainCamera = Factory::CreateUnique<Camera>();
	mainCamera->Initialize(60.0f, 0.1f, 100.0f);
	mainCamera->GetTransform()->eye = Vector3(0, 1, -3);
	mainCamera->GetTransform()->target = Vector3::Zero();
	mainCamera->ChangeCamera(Factory::Create<CameraDebugMode>(mainCamera->GetTransform()));

	lightCamera = Factory::CreateUnique<Camera>(*mainCamera.get());
	lightCamera->GetTransform()->up = Vector3::Right();

	//ライトの初期化
	lightGroup = Factory::CreateUnique<LightGroup>();
	light.direction = Vector3(-1, 1, 1).Normalize();
	light.color = Vector3::One();
	lightGroup->SetAmbientLight(Vector3::One());
	lightGroup->SetDirectionalLight(&light);

	shader = PipelineManager::GetInstance()->GetPipelineState("ToonShader");
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
	objectManager->RegisterAll();

	shader->Command();
	mainCamera->RegisterCommand();
	lightGroup->RegisterAll();
	for (int i = 0, end = (int)objects.size(); i < end; i++)
	{
		meshRenderer->Draw(*objects[i], models[i]);
	}
}

void SampleScene::Finalize()
{

}

void SampleScene::NextScene(ISceneManager* pSceneManager)
{

}

#include "TestScene.h"

void TestScene::Initialize()
{	
	//ƒJƒƒ‰‚Ì‰Šú‰»
	camera = Factory::CreateUnique<Camera>();
	camera->Initialize(60.0f, 0.1f, 100.0f);
	camera->GetTransform()->eye = Vector3(0, 0, -5);
	camera->ChangeCamera(Factory::Create<CameraDebugMode>(camera->GetTransform()));

	light.color = 1;
	light.direction = Vector3::One().Normalize();
	lightGroup = Factory::CreateUnique<LightGroup>();
	lightGroup->SetDirectionalLight(&light, 0);

	object = Factory::Create<GameObject>(Vector3::Zero());
	object->Initialize();
	//pipeline = PipelineManager::GetInstance()->GetPipelineState("FbxToonShader").lock().get();
	meshRenderer = Factory::CreateUnique<MeshRenderer>();

	mesh = ResourceManager::GetInstance()->GetMesh("char");
	anima = Factory::Create<FbxAnimation>("char");
	anima->Play(0);
}

void TestScene::Update()
{
	static float rot = 0;
	rot += 0.5f;
	object->SetRotation(Vector3(0, rot, 0));

	camera->Update();
	lightGroup->Update();
	object->Update();
	anima->Update();
}

void TestScene::Draw()
{
	pipeline->Command();
	camera->RegisterCommand();
	lightGroup->RegisterAll();
	//meshRenderer->Draw(*object, mesh, anima);
}

void TestScene::Finalize()
{
	delete(object);
	delete(anima);
}

void TestScene::NextScene(ISceneManager* pSceneManager)
{
}

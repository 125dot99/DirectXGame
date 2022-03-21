#include "GameScene1.h"

#include "ReflectCamera.h"
#include "GameCameraControl.h"

#include "WorldShip.h"
#include "WaveSimulate.h"
#include "WaveMaterial.h"

#include "dx12/Mesh.h"
#include "dx12/MeshFactory.h"

#include "CafeScene.h"
#include "SceneFade.h"
#include "Pause.h"
#include "FotMode.h"
#include "InputControl.h"

enum TEXTURE
{
	TEX_WHITE,
	TEX_TALK,
	TEX_EXIT,
	TEX_OPRATION,
	TEX_MAX,
};

bool isSceneBack;

void GameScene1::Initialize()
{
	//リソースの取得
	auto resourceManager = ResourceManager::GetInstance();
	textures.resize(TEX_MAX);
	textures[TEX_WHITE] = resourceManager->GetDefalutTexture();
	textures[TEX_TALK] = resourceManager->GetTexture("char_event.png");
	textures[TEX_EXIT] = resourceManager->GetTexture("char_event_exit.png");
	textures[TEX_OPRATION] = resourceManager->GetTexture("op_ui.png");
	
	auto playerAnima = Factory::CreateShared<FbxAnimation>("char");

	const float MAP_SIZE = 100.0f;
	ship = Factory::Create<WorldShip>(playerAnima);
	player = Factory::Create<GameObject>(Vector3(0, 0.5f, -0.1f), Vector3(0, 180, 0), Vector3::One(), ship);
	testObject = Factory::Create<GameObject>(Vector3(0, 1, -5.0f));
	auto cafe = Factory::Create<GameObject>(Vector3(0, 0, 25), Vector3(0, 180, 0));
	auto sky = Factory::Create<GameObject>(Vector3(0, -1, 0), Vector3::Zero(), Vector3(50));
	auto circle = Factory::Create<GameObject>(Vector3::Zero(), Vector3::Zero(), Vector3(37, 2, 37));
	auto wave = Factory::Create<GameObject>(Vector3::Zero(), Vector3::Zero(), Vector3(MAP_SIZE, 1, MAP_SIZE));
	objectManager = Factory::CreateUnique<ObjectManager>();
	objectManager->Add("ship", ship);
	objectManager->Add("player", player);
	objectManager->Add("sphere", testObject);
	objectManager->Add("cafe", cafe, false);
	objectManager->Add("sky", sky, false);
	objectManager->Add("circle", circle, false);
	objectManager->Add("wave", wave, false);
	cafePosition = Vector3(0, 0, 20.5f);

	collManager = Factory::CreateUnique<CollisionManager>();
	collManager->Add(ship, Factory::CreateShared<SphereCollider>(Vector3::Zero(), 1.6f), 0b01);
	collManager->Add(testObject, Factory::CreateShared<SphereCollider>(Vector3::Zero(), 1.2f), 0b11);
	objectManager->Initialize();

	rayDection = Factory::CreateUnique<EventRayDection>();
	rayDection->Add(cafePosition, 5.0f);
	if (isSceneBack)
	{
		isSceneBack = false;
		ship->SetPosition(Vector3(0, 0, 17));
	}

	//カメラの初期化
	mainCamera = Factory::CreateUnique<Camera>();
	mainCamera->Initialize(60.0f, 0.1f, 200.0f);
	mainCamera->ChangeCamera(Factory::Create<CameraFollowMode>(mainCamera->GetTransform(), ship));
	auto trans = mainCamera->GetTransform();
	trans->target = ship->GetPosition() + Vector3(0, 3, -6);
	trans->eye = trans->target - Vector3(0, 0, 6);
	reflectCamera = Factory::CreateUnique<ReflectCamera>(*mainCamera.get());

	//ライトの初期化
	lightGroup = Factory::CreateUnique<LightGroup>();
	//ゲームの時間管理 ライトの設定は関数に記述
	GameClock::SetLightGroup(lightGroup.get());

	//レンダーテクスチャ
	const Vector2 TEXTURE_SIZE(512);
	waveSimRender = Factory::CreateUnique<WaveSimulates>(ship, TEXTURE_SIZE, MAP_SIZE);
	reflectRender = Factory::CreateUnique<ReflectRender>(TEXTURE_SIZE);
	
	//描画クラス
	refRendering = Factory::CreateUnique<RenderingPipeline>();
	mainRendering = Factory::CreateUnique<RenderingPipeline>();
	enum SHADER { FBX_TOON, OBJ_TOON, SKY, WAVE, FBX_OUTLINE, OBJ_OUTLINE };
	refRendering->SetPipeline(FBX_TOON, "FbxToonShader");
	refRendering->SetPipeline(OBJ_TOON, "ToonShader");
	refRendering->SetPipeline(SKY, "SkyShader");

	refRendering->Add(OBJ_TOON, Factory::CreateShared<MeshRenderer>(ship, "ship3", "ship3"));
	refRendering->Add(OBJ_TOON, Factory::CreateShared<MeshRenderer>(testObject, "sphere", "sphere"));
	refRendering->Add(OBJ_TOON, Factory::CreateShared<MeshRenderer>(cafe, "cafe_exterior", "cafe_exterior"));
	refRendering->Add(FBX_TOON, Factory::CreateShared<MeshRenderer>(player, "char", "char", playerAnima));
	refRendering->Add(SKY, Factory::CreateShared<MeshRenderer>(sky, "hemisphere", ""));
	
	mainRendering->SetPipeline(FBX_TOON, "FbxToonShader");
	mainRendering->SetPipeline(OBJ_TOON, "ToonShader");
	mainRendering->SetPipeline(SKY, "SkyShader");
	mainRendering->SetPipeline(WAVE, "WavePatchShader");
	mainRendering->SetPipeline(FBX_OUTLINE, "FbxOutlineShader");
	mainRendering->SetPipeline(OBJ_OUTLINE, "OutlineShader");

	mainRendering->Add(OBJ_TOON, Factory::CreateShared<MeshRenderer>(ship, "ship3", "ship3"));
	mainRendering->Add(OBJ_TOON, Factory::CreateShared<MeshRenderer>(testObject, "sphere", "sphere"));
	mainRendering->Add(OBJ_TOON, Factory::CreateShared<MeshRenderer>(cafe, "cafe_exterior", "cafe_exterior"));

	mainRendering->Add(OBJ_OUTLINE, Factory::CreateShared<MeshRenderer>(ship, "ship3", ""));
	mainRendering->Add(OBJ_OUTLINE, Factory::CreateShared<MeshRenderer>(testObject, "sphere", ""));
	mainRendering->Add(OBJ_OUTLINE, Factory::CreateShared<MeshRenderer>(cafe, "cafe_exterior", ""));
	mainRendering->Add(FBX_TOON, Factory::CreateShared<MeshRenderer>(player, "char", "char", playerAnima));
	mainRendering->Add(FBX_OUTLINE, Factory::CreateShared<MeshRenderer>(player, "char", "", playerAnima));
	mainRendering->Add(SKY, Factory::CreateShared<MeshRenderer>(sky, "hemisphere", ""));
	
	//水面用のメッシュの生成
	waveMesh.reset(MeshFactory::CreatePatchPlane(16, 1.0f, 1.0f));
	//水面用のマテリアル追加
	waveMaterial = std::make_shared<WaveMaterial>();
	waveMaterial->Create();
	mainRendering->Add(WAVE, Factory::CreateShared<MeshRenderer>(wave, waveMesh, waveMaterial));
	
	spriteRenderer = Factory::CreateUnique<SpriteRenderer>();
	billboardRenderer = Factory::CreateUnique<BillboardRenderer>();

	auto heightMap = RenderManager::GetInstance()->GetRenderTarget(4);
	waveTerrainColl = Factory::CreateUnique<GpuTerrainCollider>();
	waveTerrainColl->Initialize(heightMap, { MAP_SIZE, MAP_SIZE });
	childScene = Factory::CreateUnique<SceneFadeOut>(spriteRenderer.get());
}

void GameScene1::Update()
{
	mainCamera->Update();
	reflectCamera->Update();
	childScene->Update();
	if (childScene->IsState("None"))
	{
		if (input_control::SubAction())
		{
			//ポーズに移行
			childScene = Factory::CreateUnique<FotMode>(spriteRenderer.get());
			mainCamera->ChangeCamera(Factory::Create<CameraDebugMode>(mainCamera->GetTransform()));
		} 
		else
		if (input_control::MainAction() && eventNum == (int)EVENT_ENUM::INSIDE)
		{
			childScene = Factory::CreateUnique<SceneFadeIn>(spriteRenderer.get());
		}
	}
	else if (childScene->IsNext()) //通常に戻る
	{
		if (childScene->IsState("FadeIn"))
		{
			nextSceneState = true;
		}
		else
		{
			childScene = Factory::CreateUnique<BaseChildScene>();
			mainCamera->ChangeCamera(Factory::Create<CameraFollowMode>(mainCamera->GetTransform(), ship));
		}
	}
	if (childScene->IsState("Pause") || 
		childScene->IsState("FotMode") || 
		childScene->IsState("FadeIn"))
	{
		return;
	}
	eventNum = rayDection->Update(ship, 0.7f, 180.0f);
	GameClock::Update();
	objectManager->Update();
	collManager->ChaeckAllCollisions();
}

void GameScene1::Draw()
{
	//ポーズ中、会話中でなければ描画
	bool noneState = childScene->IsState("None") || childScene->IsState("FadeOut");
	if (noneState)
	{
		waveSimRender->After();//波のテクスチャを作成
	}
	//反射用のテクスチャを作成
	reflectRender->Befoer();
	refRendering->Begin();
	reflectCamera->RegisterCommand(); //反射カメラのセット
	lightGroup->RegisterAll();
	refRendering->Draw();
	reflectRender->After();
	
	//コンピュートシェーダの判定
	waveTerrainColl->Command(testObject);
	
	//3Dモデルの描画
	mainRendering->Begin();
	mainCamera->RegisterCommand(); //通常カメラのセット
	mainRendering->Draw();

	billboardRenderer->Begin();
	if (noneState && eventNum == (int)EVENT_ENUM::INSIDE)
	{
		billboardRenderer->Draw(textures[TEX_EXIT], cafePosition + Vector3(0, 3.5f, 0), Vector2(0.8f));
	}
	billboardRenderer->End();
	
	//スプライトの描画
	spriteRenderer->Begin();
	if (!childScene->IsState("Pause") && !childScene->IsState("FotMode"))
	{
		Vector2 size = Application::GetInstance()->GetWindowSize();
		spriteRenderer->Draw(textures[TEX_OPRATION], Vector2(30, size.y - 50), Vector2::Up());
	}
		
#ifdef _DEBUG //デバック用
	auto renderManager = RenderManager::GetInstance();
	float debugSpriteSize = 256;
	spriteRenderer->DrawExtend(renderManager->GetRenderTarget(WAVE_TEX + 1), Vector2(0, 0), Vector2(debugSpriteSize));
	spriteRenderer->DrawExtend(renderManager->GetRenderTarget(NORMAL_TEX + 1), Vector2(0, debugSpriteSize * 1), Vector2(debugSpriteSize));
	spriteRenderer->DrawExtend(renderManager->GetRenderTarget(REFLECT_TEX + 1), Vector2(0, debugSpriteSize * 2), Vector2(debugSpriteSize));
#endif
	childScene->Draw();
	spriteRenderer->End();
} 
void GameScene1::Finalize()
{
	RenderManager::GetInstance()->Clear();
	isSceneBack = true;
}

void GameScene1::NextScene(ISceneManager* pSceneManager)
{
	pSceneManager->ChangeScene(new CafeScene);
}


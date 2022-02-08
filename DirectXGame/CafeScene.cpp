#include "CafeScene.h"

#include "GameScene1.h"
#include "MiniGameScene.h"
#include "Pause.h"
#include "SceneFade.h"

#include "Talk.h"
#include "Player.h"
#include "GameCameraControl.h"

enum MODEL
{
	PLAYER,
	MISTRESS,
	SOFA,
	TABLE,
	GAME_TABLE,
	CAFE,
	MAX,
};

enum TEXTURE
{
	TEX_WHITE,
	TEX_OPRATION,
	TEX_TALK,
	TEX_EXIT,
	TEX_MAX,
};

bool isWorld;
bool isGame;
bool isTalk;

void CafeScene::Initialize()
{
	//リソースの取得
	auto resourceManager = ResourceManager::GetInstance();
	models.resize(MAX);
	models[PLAYER] = resourceManager->GetModel("char");
	models[MISTRESS] = resourceManager->GetModel("mistress");
	models[SOFA] = resourceManager->GetModel("sofa");
	models[TABLE] = resourceManager->GetModel("table2");
	models[GAME_TABLE] = resourceManager->GetModel("game_table");
	models[CAFE] = resourceManager->GetModel("cafe_interior");

	textures.resize(TEX_MAX);
	textures[TEX_WHITE] = resourceManager->GetDefalutTexture();
	textures[TEX_OPRATION] = resourceManager->GetTexture("op_ui.png");
	textures[TEX_TALK] = resourceManager->GetTexture("char_event.png");
	textures[TEX_EXIT] = resourceManager->GetTexture("char_event_exit.png");

	playerAnima = Factory::CreateUnique<FbxAnimation>(models[PLAYER].lock().get());
	mistressAnima = Factory::CreateUnique<FbxAnimation>(models[MISTRESS].lock().get());
	mistressAnima->Play(0);

	//ゲームオブジェクトの初期化
	objectManager = Factory::CreateUnique<ObjectManager>();

	player = Factory::Create<Player>(playerAnima.get());
	mistress = Factory::Create<GameObject>(Vector3(-4, 0, 2));
	sofaObjects[0] = Factory::Create<GameObject>(Vector3(2, 0, 3), Vector3(0, 90, 0));
	sofaObjects[1] = Factory::Create<GameObject>(Vector3(5, 0, 3), Vector3(0, -90, 0));
	sofaObjects[2] = Factory::Create<GameObject>(Vector3(2, 0, 0), Vector3(0, 90, 0));
	sofaObjects[3] = Factory::Create<GameObject>(Vector3(5, 0, 0), Vector3(0, -90, 0));
	table = Factory::Create<GameObject>(Vector3(3.5f, 0, 0), Vector3(0, -90, 0));
	game_table = Factory::Create<GameObject>(Vector3(3.5f, 0, 3), Vector3(0, 90, 0));
	cafe = Factory::Create<GameObject>(Vector3(0, 0, 1));

	objectManager->Add(player);
	objectManager->Add(mistress);

	objectManager->Add(table, false);
	objectManager->Add(game_table, false);
	objectManager->Add(cafe, false);
	for (auto&& x : sofaObjects)
	{
		objectManager->Add(x, false);
	}
	objectManager->Initialize();

	collManager = Factory::CreateUnique<CollisionManager>();
	collManager->AddCollider(player, Factory::CreateShared<SphereCollider>(Vector3::Zero(), 0.6f), 0b01);
	collManager->AddCollider(mistress, Factory::CreateShared<SphereCollider>(Vector3::Zero(), 0.6f), 0b11);

	//カメラの初期化
	mainCamera = Factory::CreateUnique<Camera>();
	mainCamera->Initialize(60.0f, 0.01f, 100.0f);
	mainCamera->ChangeCamera(Factory::Create<CameraTargetMode>(mainCamera->GetTransform(), player));

	dirLight1.direction = Vector3(0, 0, -1).Normalize();
	dirLight1.color = Vector4(0.5f, 0.5f, 0.5f, 0);

	//ライトの初期化
	lightGroup = Factory::CreateUnique<LightGroup>();
	lightGroup->SetAmbientLight(Vector3(0.8f, 0.76f, 0.6f));
	lightGroup->SetDirectionalLight(&dirLight1, 0);
	
	//シェーダーを参照
	auto pipelineManager = PipelineManager::GetInstance();
	fbxShader = pipelineManager->GetPipelineState("FbxToonShader");
	toonShader = pipelineManager->GetPipelineState("ToonShader");
	outlineShader = pipelineManager->GetPipelineState("OutlineShader");
	fbxOutlineShader = pipelineManager->GetPipelineState("FbxOutlineShader");

	//描画クラス
	meshRenderer = Factory::CreateUnique<MeshRenderer>();
	spriteRenderer = Factory::CreateUnique<SpriteRenderer>();
	billboardRenderer = Factory::CreateUnique<BillboardRenderer>();

	childScene = Factory::CreateUnique<SceneFadeOut>(spriteRenderer.get());
	debugRay = Factory::CreateUnique<DebugRay>();

	nextSceneEnum = NEXT_SCENE_STATE::NONE;
}

void CafeScene::Update()
{
	mainCamera->Update();
	childScene->Update();

	if (childScene->IsState("None"))
	{
		if (Input::GetKeyborad()->IsKeyDown(KEY_CODE::X) ||
			Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::X))
		{
			//ポーズに移行
			childScene = Factory::CreateUnique<Pause>(spriteRenderer.get());
			mainCamera->ChangeCamera(Factory::Create<CameraStaticMode>());
		}
		//else
		//if ((Input::GetKeyborad()->IsKeyDown(KEY_CODE::SPACE) ||
		//	Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::B)) && 
		//	isTalk)
		//{
		//	childScene = Factory::CreateUnique<Talk>(spriteRenderer.get(), "test4.gct");
		//}
	} 
	else if (childScene->IsNext()) //通常に戻る
	{
		if (childScene->IsState("FadeOut"))
		{
			childScene = Factory::CreateUnique<BaseChildScene>();
		}
		else if (childScene->IsState("FadeIn"))
		{
			nextSceneState = true;
		}
		else
		{
			childScene = Factory::CreateUnique<BaseChildScene>();
			mainCamera->ChangeCamera(Factory::Create<CameraTargetMode>(mainCamera->GetTransform(), player));
		}
	}
	if (childScene->IsState("Pause") ||
		childScene->IsState("Talk") || 
		childScene->IsState("FadeIn"))
	{
		return;
	}

	if (Input::GetKeyborad()->IsKeyDown(KEY_CODE::SPACE) ||
		Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::B))
	{
		nextSceneEnum = isWorld ? NEXT_SCENE_STATE::WORLD : isGame ? NEXT_SCENE_STATE::GAME : NEXT_SCENE_STATE::NONE;
		if (nextSceneEnum != NEXT_SCENE_STATE::NONE)
		{
			//フェードインに入る
			childScene = Factory::CreateUnique<SceneFadeIn>(spriteRenderer.get());
		}
	}

	isWorld = (-Vector3::Forward() - player->GetPosition()).Length() < 0.2f;
	isGame = (game_table->GetPosition() - player->GetPosition()).Length() < 10.0f;
	isTalk = (mistress->GetPosition() - player->GetPosition()).Length() < 5.0f;
	
	GameClock::Update();
	
	objectManager->Update();
	collManager->ChaeckAllCollisions();
}

void CafeScene::Draw()
{
	objectManager->RegisterAll();

	//ポーズ中、会話中でなければ描画
	bool noneState = childScene->IsState("None") || childScene->IsState("FadeOut");
	if (noneState)
	{
		playerAnima->Update();
		mistressAnima->Update();
	}
	//モデルの描画
	fbxShader->Command();
	mainCamera->RegisterCommand(); //通常カメラのセット
	lightGroup->RegisterAll(); //ライトのセット
	meshRenderer->Draw(*player, models[PLAYER], playerAnima.get());
	meshRenderer->Draw(*mistress, models[MISTRESS], mistressAnima.get());
	toonShader->Command();
	for (auto&& x : sofaObjects)
	{
		meshRenderer->Draw(*x, models[SOFA]);
	}
	meshRenderer->Draw(*table, models[TABLE]);
	meshRenderer->Draw(*game_table, models[GAME_TABLE]);
	meshRenderer->Draw(*cafe, models[CAFE]);

	//アウトライン
	fbxOutlineShader->Command();
	meshRenderer->Draw(*player, models[PLAYER], playerAnima.get());
	meshRenderer->Draw(*mistress, models[MISTRESS], mistressAnima.get());
	outlineShader->Command();
	for (auto&& x : sofaObjects)
	{
		meshRenderer->Draw(*x, models[SOFA], false);
	}
	meshRenderer->Draw(*table, models[TABLE], false);
	meshRenderer->Draw(*game_table, models[GAME_TABLE], false);
	
	billboardRenderer->Begin();
	if (childScene->IsState("None"))
	{
		if (isWorld)
		{
			billboardRenderer->Draw(textures[TEX_EXIT], Vector3(0, 3, -1), Vector2(0.4f));
		}
		else if (isGame)
		{
			billboardRenderer->Draw(textures[TEX_TALK], game_table->GetPosition() + Vector3(0, 2, 0), Vector2(0.7f));
		}
		//else if (isTalk)
		//{
		//	billboardRenderer->Draw(textures[TEX_TALK], mistress->GetPosition() + Vector3(0, 2.5f, 0), Vector2(0.6f));
		//}
	}
	billboardRenderer->End();

	//スプライトの描画
	spriteRenderer->Begin();
	if (noneState)
	{
		Vector2 size = Application::GetInstance()->GetWindowSize();
		spriteRenderer->Draw(textures[TEX_OPRATION], Vector2(30, size.y - 30), Vector2::Up(), Vector4(1, 1, 1, 1));
	}
	childScene->Draw();
	spriteRenderer->End();
}

void CafeScene::Finalize()
{
	models.clear();
	textures.clear();
}

void CafeScene::NextScene(ISceneManager* pSceneManager)
{
	switch (nextSceneEnum)
	{
	case CafeScene::NEXT_SCENE_STATE::NONE:
		break;
	case CafeScene::NEXT_SCENE_STATE::WORLD:
		pSceneManager->ChangeScene(new GameScene1);
		break;
	case CafeScene::NEXT_SCENE_STATE::GAME:
		pSceneManager->ChangeScene(new MiniGameScene);
		break;
	default:
		break;
	}
}

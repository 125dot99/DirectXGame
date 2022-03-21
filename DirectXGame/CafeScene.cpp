#include "CafeScene.h"

#include "GameScene1.h"
#include "MiniGameScene.h"
#include "Pause.h"
#include "FotMode.h"
#include "SceneFade.h"

#include "Talk.h"
#include "Player.h"
#include "Mistress.h"
#include "GameCameraControl.h"
#include "InputControl.h"

enum TEXTURE
{
	TEX_WHITE,
	TEX_OPRATION,
	TEX_TALK,
	TEX_EXIT,
	TEX_GAME_TABLE,
	TEX_MAX,
};

//プレイヤーの座標と向きを保持する
Vector3 position;
Vector3 rotation;

void CafeScene::Initialize()
{
	//リソースの取得
	auto resourceManager = ResourceManager::GetInstance();
	textures.resize(TEX_MAX);
	textures[TEX_WHITE] = resourceManager->GetDefalutTexture();
	textures[TEX_OPRATION] = resourceManager->GetTexture("op_ui.png");
	textures[TEX_TALK] = resourceManager->GetTexture("char_event.png");
	textures[TEX_EXIT] = resourceManager->GetTexture("char_event_exit.png");
	textures[TEX_GAME_TABLE] = resourceManager->GetTexture("char_event_game.png");

	auto playerAnima = Factory::CreateShared<FbxAnimation>("char");
	auto mistressAnima = Factory::CreateShared<FbxAnimation>("mistress");

	std::shared_ptr<TalkObserver> playerObserver = Factory::CreateShared<TalkObserver>();
	std::shared_ptr<TalkObserver> mistressObserver = Factory::CreateShared<TalkObserver>();
	talkSubject = Factory::CreateUnique<TalkSubject>();
	talkSubject->Register(playerObserver);
	talkSubject->Register(mistressObserver);

	//ゲームオブジェクトの初期化
	objectManager = Factory::CreateUnique<ObjectManager>();
	player = Factory::Create<Player>(playerAnima, playerObserver);
	mistress = Factory::Create<Mistress>(mistressAnima, mistressObserver);
	GameObject* boxObject = Factory::Create<GameObject>(Vector3::Zero());
	objectManager->Add("player", player);
	objectManager->Add("mistress", mistress);
	objectManager->Add("box_interior", boxObject, false);

	//データ読み込み
	auto levelData = JsonLevelLoader::LoadFile("cafescene");
	std::vector<GameObject*> objects((int)levelData.objects.size());
	for (int i = 0, end = (int)levelData.objects.size(); i < end; i++)
	{
		Vector3 pos = levelData.objects[i].translation;
		Vector3 rot = levelData.objects[i].rotation;
		Vector3 scale = levelData.objects[i].scaling;
		std::string fileName = levelData.objects[i].fileName;
		objects[i] = Factory::Create<GameObject>(pos, rot, scale);
		objectManager->Add(fileName, objects[i], false);
	}
	objectManager->Initialize();
	if (position.Length() != 0)
	{
		player->SetPosition(position);
		player->SetRotation(rotation);
		position = rotation = {};
	}
	game_tablePos = objectManager->Find("game_table")->GetPosition();

	//衝突マネージャー
	collManager = Factory::CreateUnique<CollisionManager>();
	collManager->Add(player, Factory::CreateShared<SphereCollider>(Vector3::Zero(), 0.6f), 0b01);
	collManager->Add(mistress, Factory::CreateShared<SphereCollider>(Vector3::Zero(), 0.6f), 0b11);
	
	rayDection = Factory::CreateUnique<EventRayDection>();
	rayDection->Add(Vector3(0, 0, -1.5f), 1.0f);
	rayDection->Add(game_tablePos, 10.0f);
	//rayDection->Add(mistress->GetPosition(), 5.0f);

	//カメラの初期化
	mainCamera = Factory::CreateUnique<Camera>();
	mainCamera->Initialize(60.0f, 0.01f, 50.0f);
	mainCamera->ChangeCamera(Factory::Create<CameraTargetMode>(mainCamera->GetTransform(), player));

	//ライトの初期化
	dirLight1.direction = Vector3(0, 0, -1).Normalize();
	dirLight1.color = Vector4(0.5f, 0.5f, 0.5f, 0);
	lightGroup = Factory::CreateUnique<LightGroup>();
	lightGroup->SetAmbientLight(Vector3(0.9f, 0.84f, 0.7f));
	lightGroup->SetDirectionalLight(&dirLight1, 0);
	
	//描画クラス
	rendering = Factory::CreateUnique<RenderingPipeline>();
	enum SHADER { FBX_TOON, OBJ_TOON, FBX_OUTLINE, OBJ_OUTLINE };
	rendering->SetPipeline(FBX_TOON, "FbxToonShader");
	rendering->SetPipeline(FBX_OUTLINE, "FbxOutlineShader");
	rendering->SetPipeline(OBJ_TOON, "ToonShader");
	rendering->SetPipeline(OBJ_OUTLINE, "OutlineShader");
	rendering->Add(FBX_TOON, Factory::CreateShared<MeshRenderer>(player, "char", "char", playerAnima));
	rendering->Add(FBX_OUTLINE, Factory::CreateShared<MeshRenderer>(player, "char", "", playerAnima));
	rendering->Add(FBX_TOON, Factory::CreateShared<MeshRenderer>(mistress, "mistress", "mistress", mistressAnima));
	rendering->Add(FBX_OUTLINE, Factory::CreateShared<MeshRenderer>(mistress, "mistress", "", mistressAnima));
	rendering->Add(OBJ_TOON, Factory::CreateShared<MeshRenderer>(boxObject, "cafe_interior", "cafe_interior"));
	for (int i = 0, end = (int)objects.size(); i < end; i++)
	{
		std::string fileName = levelData.objects[i].fileName;
		rendering->Add(OBJ_TOON, Factory::CreateShared<MeshRenderer>(objects[i], fileName, fileName));
		rendering->Add(OBJ_OUTLINE, Factory::CreateShared<MeshRenderer>(objects[i], fileName, ""));
	}

	//影
	u_pConstBufferShadow = std::make_unique<ConstBuffer>();
	u_pConstBufferShadow->Init((UINT)ROOT_PARAMETER::MATERIAL, sizeof(Matrix4));
	Matrix4 m = MatrixShadow(Vector3(-1, 5, -3), 0);
	u_pConstBufferShadow->Map(&m);

	shadowRendering = Factory::CreateUnique<RenderingPipeline>();
	shadowRendering->SetPipeline(0, "Shadow");
	for (int i = 0, end = (int)objects.size(); i < end; i++)
	{
		std::string fileName = levelData.objects[i].fileName;
		shadowRendering->Add(0, Factory::CreateShared<MeshRenderer>(objects[i], fileName, ""));
	}

	//描画クラス
	spriteRenderer = Factory::CreateUnique<SpriteRenderer>();
	billboardRenderer = Factory::CreateUnique<BillboardRenderer>();

	childScene = Factory::CreateUnique<SceneFadeOut>(spriteRenderer.get());
}

void CafeScene::Update()
{
	mainCamera->Update();
	childScene->Update();
	if (childScene->IsState("None"))
	{
		if (input_control::MainAction())
		{
			if (eventNum == (int)EVENT_ENUM::TALK)
			{
				childScene = Factory::CreateUnique<Talk>(spriteRenderer.get(), "cafe_text.gct");
				Vector3 p = rayDection->GetEventPos(eventNum);
				//talkSubject->Notify(true);
			}
			else if (eventNum == (int)EVENT_ENUM::OUTER || eventNum == (int)EVENT_ENUM::MINIGAME)
			{
				//フェードインに入る
				childScene = Factory::CreateUnique<SceneFadeIn>(spriteRenderer.get());
			}
		}
		if (input_control::SubAction())
		{
			//ポーズに移行
			childScene = Factory::CreateUnique<FotMode>(spriteRenderer.get());
			mainCamera->ChangeCamera(Factory::Create<CameraStaticMode>());
		}
	} 
	else if (childScene->IsNext()) //通常に戻る
	{
		if (childScene->IsState("FadeIn"))
		{
			if (eventNum == (int)EVENT_ENUM::TALK)
			{
				childScene = Factory::CreateUnique<SceneNextCafeWait>(spriteRenderer.get(), false);
			}
			else
			{
				nextSceneState = true;
			}
		}
		else if (childScene->IsState("Talk"))
		{
 			childScene = Factory::CreateUnique<SceneFadeIn>(spriteRenderer.get());
		}
		else if (childScene->IsState("NextWait"))
		{
			childScene = Factory::CreateUnique<SceneFadeOut>(spriteRenderer.get());
		}
		else
		{
			childScene = Factory::CreateUnique<BaseChildScene>();
			mainCamera->ChangeCamera(Factory::Create<CameraTargetMode>(mainCamera->GetTransform(), player));
			eventNum = (int)EVENT_ENUM::NONE;
		}
	}
	if (childScene->IsState("Pause") || 
		childScene->IsState("FotMode") ||
		childScene->IsState("FadeIn"))
	{
		return;
	}
	eventNum = rayDection->Update(player, 0.7f);
	GameClock::Update();
	objectManager->Update();
	collManager->ChaeckAllCollisions();
}

void CafeScene::Draw()
{
	rendering->Begin();
	//通常カメラのセット
	mainCamera->RegisterCommand(); 
	//ライトのセット
	lightGroup->RegisterAll();
	//モデルの描画
	rendering->Draw();

	shadowRendering->Begin();
	u_pConstBufferShadow->GraphicsCommand();
	shadowRendering->Draw();

	billboardRenderer->Begin();
	if (childScene->IsState("None"))
	{
		switch (eventNum)
		{
		case (int)EVENT_ENUM::OUTER:
			billboardRenderer->Draw(textures[TEX_EXIT], Vector3(0, 3, -1), Vector2(0.4f));
			break;
		case (int)EVENT_ENUM::MINIGAME:
			billboardRenderer->Draw(textures[TEX_GAME_TABLE], game_tablePos + Vector3(0, 2, 0), Vector2(0.7f));
			break;
		case (int)EVENT_ENUM::TALK:
			billboardRenderer->Draw(textures[TEX_TALK], mistress->GetPosition() + Vector3(0, 2.5f, 0), Vector2(0.6f));
			break;
		default:
			break;
		}
	}
	billboardRenderer->End();

	//スプライトの描画
	spriteRenderer->Begin();
	if (!childScene->IsState("Pause") && !childScene->IsState("FotMode"))
	{
		Vector2 size = Application::GetInstance()->GetWindowSize();
		spriteRenderer->Draw(textures[TEX_OPRATION], Vector2(30, size.y - 50), Vector2::Up());
	}
	childScene->Draw();
	spriteRenderer->End();
}

void CafeScene::Finalize()
{
	RenderManager::GetInstance()->Clear();
	textures.clear();
}

void CafeScene::NextScene(ISceneManager* pSceneManager)
{
	switch (eventNum)
	{
	case (int)EVENT_ENUM::OUTER:
		pSceneManager->ChangeScene(new GameScene1);
		break;
	case (int)EVENT_ENUM::MINIGAME:
		position = player->GetPosition();//シーンに入る前の座標を保存
		rotation = player->GetRotation();
		pSceneManager->ChangeScene(new MiniGameScene);
		break;
	default:
		break;
	}
}

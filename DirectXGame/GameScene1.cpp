#include "GameScene1.h"
#include "WorldShip.h"
#include "ReflectCamera.h"
#include "WaveSimulate.h"
#include "GameCameraControl.h"
#include "CafeScene.h"
#include "SceneFade.h"
#include "Pause.h"

enum MODEL
{
	SHIP,
	PLAYER,
	CAFE,
	SPHERE,
	SKY,
	CIRCLE,
	MAX,
};

enum TEXTURE
{
	TEX_WHITE,
	TEX_TALK,
	TEX_EXIT,
	TEX_OPRATION,
	TEX_PAUSE,
	TEX_MAX,
};

bool isSceneBack;
bool isInCafe;

void GameScene1::Initialize()
{
	//���\�[�X�̎擾
	auto resourceManager = ResourceManager::GetInstance();
	models.resize(MAX);
	models[SHIP] = resourceManager->GetModel("ship3");
	models[PLAYER] = resourceManager->GetModel("char");
	models[CAFE] = resourceManager->GetModel("cafe_exterior");
	models[SPHERE] = resourceManager->GetModel("sphere");
	models[SKY] = resourceManager->GetModel("hemisphere");
	models[CIRCLE] = resourceManager->GetModel("circle_wall");

	textures.resize(TEX_MAX);
	textures[TEX_WHITE] = resourceManager->GetDefalutTexture();
	textures[TEX_TALK] = resourceManager->GetTexture("char_event.png");
	textures[TEX_EXIT] = resourceManager->GetTexture("char_event_exit.png");
	textures[TEX_OPRATION] = resourceManager->GetTexture("op_ui.png");
	textures[TEX_PAUSE] = resourceManager->GetTexture("pause_ui.png");

	playerAnima = Factory::CreateUnique<FbxAnimation>(models[PLAYER].lock().get());

	const float MAP_SIZE = 100.0f;
	ship = Factory::Create<WorldShip>(playerAnima.get());
	player = Factory::Create<GameObject>(Vector3(0, 0.5f, -0.1f), Vector3(0, 180, 0), Vector3::One(), ship);
	testObject = Factory::Create<GameObject>(Vector3(0, 1, -5.0f));
	cafe = Factory::Create<GameObject>(Vector3(0, 0, 25), Vector3(0, 180, 0));
	skydome = Factory::Create<GameObject>(Vector3(0, -1, 0), Vector3::Zero(), Vector3(50));
	wave = Factory::Create<GameObject>(Vector3::Zero(), Vector3::Zero(), Vector3(MAP_SIZE, 1, MAP_SIZE));
	circle = Factory::Create<GameObject>(Vector3::Zero(), Vector3::Zero(), Vector3(37, 2, 37));
	
	objectManager = Factory::CreateUnique<ObjectManager>();
	objectManager->Add(ship);
	objectManager->Add(player);
	objectManager->Add(testObject);
	objectManager->Add(cafe, false);
	objectManager->Add(skydome, false);
	objectManager->Add(wave, false);
	objectManager->Add(circle, false);
	collManager = Factory::CreateUnique<CollisionManager>();
	collManager->AddCollider(ship, Factory::CreateShared<SphereCollider>(Vector3::Zero(), 1.6f), 0b01);
	collManager->AddCollider(testObject, Factory::CreateShared<SphereCollider>(Vector3::Zero(), 1.2f), 0b11);
	
	objectManager->Initialize();

	if (isSceneBack)
	{
		isSceneBack = false;
		ship->SetPosition(Vector3(0, 0, 15));
	}

	//�J�����̏�����
	mainCamera = Factory::CreateUnique<Camera>();
	mainCamera->Initialize(60.0f, 0.1f, 200.0f);
	mainCamera->ChangeCamera(Factory::Create<CameraFollowMode>(mainCamera->GetTransform(), ship));
	auto trans = mainCamera->GetTransform();
	trans->target = ship->GetPosition() + Vector3(0, 3, -6);
	trans->eye = trans->target - Vector3(0, 0, 6);
	reflectCamera = Factory::CreateUnique<ReflectCamera>(*mainCamera.get());

	//���C�g�̏�����
	lightGroup = Factory::CreateUnique<LightGroup>();
	//�Q�[���̎��ԊǗ�
	GameClock::SetLightGroup(lightGroup.get());

	//�����_�[�e�N�X�`��
	const Vector2 TEXTURE_SIZE(512);
	waveSimu =  Factory::CreateUnique<WaveSimulates>(ship, TEXTURE_SIZE, MAP_SIZE);
	reflectTexture = Factory::CreateUnique<ReflectRender>(TEXTURE_SIZE);
	
	//�V�F�[�_�[���Q��
	auto pipelineManager = PipelineManager::GetInstance();
	fbxShader = pipelineManager->GetPipelineState("FbxToonShader");
	toonShader = pipelineManager->GetPipelineState("ToonShader");
	skyShader = pipelineManager->GetPipelineState("SkyShader");
	outlineShader = pipelineManager->GetPipelineState("OutlineShader");
	fbxOutlineShader = pipelineManager->GetPipelineState("FbxOutlineShader");

	//�`��N���X
	meshRenderer = Factory::CreateUnique<MeshRenderer>();
	spriteRenderer = Factory::CreateUnique<SpriteRenderer>();
	billboardRenderer = Factory::CreateUnique<BillboardRenderer>();
	ray = Factory::CreateUnique<DebugRay>();

	//auto paTex = resourceManager->GetTexture("effect1.png");
	//particle = Factory::CreateUnique<ParticleUnit>(paTex);
	//particleManager = Factory::CreateUnique<ParticleManager>();
	//particleManager->Add(particle.get());

	auto texture = RenderManager::GetInstance()->GetRenderTarget(4);
	waveTerrainColl = Factory::CreateUnique<GpuTerrainCollider>();
	waveTerrainColl->Initialize(texture, { MAP_SIZE, MAP_SIZE });
	waveRender = Factory::CreateUnique<Wave>(wave);

	childScene = Factory::CreateUnique<SceneFadeOut>(spriteRenderer.get());
}

void GameScene1::Update()
{
	mainCamera->Update();
	reflectCamera->Update();

	childScene->Update();
	if (childScene->IsState("None"))
	{
		isInCafe = (cafe->GetPosition() + Vector3(0, 0, -4) - ship->GetPosition()).Length() < 10.0f;
		if (Input::GetKeyborad()->IsKeyDown(KEY_CODE::X) ||
			Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::X))
		{
			//�|�[�Y�Ɉڍs
			childScene = Factory::CreateUnique<Pause>(spriteRenderer.get());
			mainCamera->ChangeCamera(Factory::Create<CameraDebugMode>(mainCamera->GetTransform()));
		} 
		else
		if (Input::GetKeyborad()->IsKeyDown(KEY_CODE::SPACE) ||
			Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::B))
		{
			if (isInCafe)
			{
				childScene = Factory::CreateUnique<SceneFadeIn>(spriteRenderer.get());
			}
		}
	}
	else if (childScene->IsNext()) //�ʏ�ɖ߂�
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
			mainCamera->ChangeCamera(Factory::Create<CameraFollowMode>(mainCamera->GetTransform(), ship));
		}
	}
	if (childScene->IsState("Pause") || 
		childScene->IsState("FadeIn"))
	{
		return;
	}

	GameClock::Update();
	
	objectManager->Update();
	collManager->ChaeckAllCollisions();
	
	if (Input::GetKeyborad()->IsKeyDown(KEY_CODE::_0) ||
		Input::GetGamepad()->IsButtonDown(GAMEPAD_CODE::BACK))
	{
		childScene = Factory::CreateUnique<SceneFadeIn>(spriteRenderer.get());
	}
}

void GameScene1::Draw()
{
	objectManager->RegisterAll();
	//�|�[�Y���A��b���łȂ���Ε`��
	bool noneState = childScene->IsState("None") || childScene->IsState("FadeOut");
	if (noneState)
	{
		waveSimu->After();//�g�̃e�N�X�`�����쐬
		playerAnima->Update();
	}
	//���˗p�̃e�N�X�`�����쐬
	reflectTexture->Befoer();
	{
		skyShader->Command();
		reflectCamera->RegisterCommand(); //���˃J�����̃Z�b�g
		lightGroup->RegisterAll();
		meshRenderer->Draw(*skydome, models[SKY]);
		fbxShader->Command();
		meshRenderer->Draw(*player, models[PLAYER], playerAnima.get());
		toonShader->Command();
		meshRenderer->Draw(*ship, models[SHIP]);
		meshRenderer->Draw(*testObject, models[SPHERE]);
		meshRenderer->Draw(*cafe, models[CAFE]);
	}
	reflectTexture->After();
	
	//�R���s���[�g�V�F�[�_�̔���
	waveTerrainColl->Command(testObject);
	//3D���f���̕`��
	{
		skyShader->Command();
		mainCamera->RegisterCommand(); //�ʏ�J�����̃Z�b�g
		meshRenderer->Draw(*skydome, models[SKY]);
		fbxShader->Command();
		meshRenderer->Draw(*player, models[PLAYER], playerAnima.get());
		toonShader->Command();
		meshRenderer->Draw(*ship, models[SHIP]);
		meshRenderer->Draw(*testObject, models[SPHERE]);
		meshRenderer->Draw(*cafe, models[CAFE]);
		//�g�̕`��
		waveRender->Draw();
		
		//�A�E�g���C��
		fbxOutlineShader->Command();
		meshRenderer->Draw(*player, models[PLAYER], playerAnima.get(), false);
		outlineShader->Command();
		meshRenderer->Draw(*ship, models[SHIP], false);
		meshRenderer->Draw(*testObject, models[SPHERE], false);
		meshRenderer->Draw(*cafe, models[CAFE], false);
	}

	billboardRenderer->Begin();
	if (noneState)
	{
		if (isInCafe)
		{
			billboardRenderer->Draw(textures[TEX_EXIT], cafe->GetPosition() + Vector3(0, 3.5f, -4), Vector2(0.8f));
		}
	}
	billboardRenderer->End();
	
	//�X�v���C�g�̕`��
	spriteRenderer->Begin();
	if (noneState)
	{
		Vector2 size = Application::GetInstance()->GetWindowSize();
		spriteRenderer->Draw(textures[TEX_OPRATION], Vector2(30, size.y - 30), Vector2::Up(), Vector4(1, 1, 1, 1));
	}
	childScene->Draw();
	spriteRenderer->End();
}

void GameScene1::Finalize()
{
	RenderManager::GetInstance()->Clear();
	models.clear();
	textures.clear();
	isSceneBack = true;
}

void GameScene1::NextScene(ISceneManager* pSceneManager)
{
	pSceneManager->ChangeScene(new CafeScene);
}


#pragma once
#include <GameLibHeader.h>

#include "GameClock.h"
#include "GpuTerrainCollider.h"
#include "EventRayDection.h"

using namespace gamelib;
class GameScene1 : public BaseScene
{
private:
	//���C���J����
	std::unique_ptr<Camera> mainCamera;
	//���˃J����
	std::unique_ptr<Camera> reflectCamera;

	//���C�g
	std::unique_ptr<LightGroup> lightGroup;
	//UI��t�F�[�h�C���t�F�[�h�A�E�g
	std::unique_ptr<BaseChildScene> childScene;

	Vector3 cafePosition;
	GameObject* ship;
	GameObject* player;
	GameObject* testObject;
	std::unique_ptr<ObjectManager> objectManager;
	std::unique_ptr<CollisionManager> collManager;

	std::vector<std::weak_ptr<Texture>> textures;
	std::unique_ptr<IPostProcess> waveSimRender;
	std::unique_ptr<IPostProcess> reflectRender;

	enum class EVENT_ENUM
	{
		NONE,
		INSIDE,
	};
	int eventNum = (int)EVENT_ENUM::NONE;
	std::unique_ptr<EventRayDection> rayDection;

	//���ʗp�̃��b�V��
	std::shared_ptr<IMesh> waveMesh;
	std::shared_ptr<IMaterial> waveMaterial;
	std::unique_ptr<RenderingPipeline> mainRendering;
	std::unique_ptr<RenderingPipeline> refRendering;
	std::unique_ptr<SpriteRenderer> spriteRenderer;
	std::unique_ptr<BillboardRenderer> billboardRenderer;

	std::unique_ptr<GpuTerrainCollider> waveTerrainColl;

	std::unique_ptr<ParticleUnit> particle;
	std::unique_ptr<ParticleManager> particleManager;
	std::unique_ptr<DebugRay> ray;
	
	bool debugSpriteEnable = false;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void NextScene(ISceneManager* pSceneManager) override;
};

#pragma once
#include <GameLibHeader.h>
#include "GameClock.h"
#include "GpuTerrainCollider.h"
#include "Wave.h"

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
	std::unique_ptr<BaseChildScene> childScene;
	std::unique_ptr<ObjectManager> objectManager;
	std::unique_ptr<CollisionManager> collManager;

	GameObject* ship;
	GameObject* player;
	GameObject* cafe;
	GameObject* testObject;
	GameObject* skydome;
	GameObject* wave;
	GameObject* circle;

	std::unique_ptr<FbxAnimation> playerAnima;

	//���\�[�X
	std::vector<std::weak_ptr<IMesh>> models;
	std::vector<std::weak_ptr<Texture>> textures;

	std::unique_ptr<IPostProcess> waveSimu;
	std::unique_ptr<IPostProcess> reflectTexture;

	IPipelineState* fbxShader;
	IPipelineState* toonShader;
	IPipelineState* skyShader;//��V�F�[�_�[
	IPipelineState* fbxOutlineShader;
	IPipelineState* outlineShader;
	
	std::unique_ptr<MeshRenderer> meshRenderer;
	std::unique_ptr<SpriteRenderer> spriteRenderer;
	std::unique_ptr<BillboardRenderer> billboardRenderer;

	std::unique_ptr<Wave> waveRender;
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

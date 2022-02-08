#pragma once
#include <GameLibHeader.h>

using namespace gamelib;
class CafeScene : public BaseScene
{
private:
	enum class NEXT_SCENE_STATE
	{
		NONE,
		WORLD,
		GAME,
	};

	NEXT_SCENE_STATE nextSceneEnum;

	std::unique_ptr<Camera> mainCamera;//ÉÅÉCÉìÉJÉÅÉâ
	
	DirectionalLight dirLight1;
	SpotLight spotlight;
	std::unique_ptr<LightGroup> lightGroup;
	std::unique_ptr<BaseChildScene> childScene;
	
	GameObject* player;
	GameObject* mistress;

	GameObject* sofaObjects[4];
	GameObject* table;
	GameObject* game_table;
	GameObject* cafe;

	std::unique_ptr<ObjectManager> objectManager;
	std::unique_ptr<CollisionManager> collManager;

	std::vector<std::weak_ptr<IMesh>> models;
	std::vector<std::weak_ptr<Texture>> textures;
	
	std::unique_ptr<FbxAnimation> playerAnima;
	std::unique_ptr<FbxAnimation> mistressAnima;

	IPipelineState* fbxShader;
	IPipelineState* toonShader;
	IPipelineState* fbxOutlineShader;
	IPipelineState* outlineShader;

	std::unique_ptr<MeshRenderer> meshRenderer;
	std::unique_ptr<SpriteRenderer> spriteRenderer;
	std::unique_ptr<BillboardRenderer> billboardRenderer;

	std::unique_ptr<DebugRay> debugRay;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void NextScene(ISceneManager* pSceneManager) override;
};


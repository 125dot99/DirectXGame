#pragma once
#include <GameLibHeader.h>
#include "EventRayDection.h"
#include "TalkObserver.h"

using namespace gamelib;
class CafeScene : public BaseScene
{
private:
	//メインカメラ
	std::unique_ptr<Camera> mainCamera; 
	
	DirectionalLight dirLight1;
	std::unique_ptr<LightGroup> lightGroup;
	//UIやフェードインフェードアウト
	std::unique_ptr<BaseChildScene> childScene;

	GameObject* player;
	GameObject* mistress;
	Vector3 game_tablePos;
	enum class EVENT_ENUM
	{
		NONE,
		OUTER,
		MINIGAME,
		TALK,
	};

	int eventNum = (int)EVENT_ENUM::NONE;
	std::unique_ptr<EventRayDection> rayDection;
	std::unique_ptr<TalkSubject> talkSubject;

	std::unique_ptr<ObjectManager> objectManager;
	std::unique_ptr<CollisionManager> collManager;

	std::vector<std::weak_ptr<Texture>> textures;
	std::unique_ptr<RenderingPipeline> rendering;
	std::unique_ptr<ConstBuffer> u_pConstBufferShadow;
	std::unique_ptr<RenderingPipeline> shadowRendering;
	std::unique_ptr<SpriteRenderer> spriteRenderer;
	std::unique_ptr<BillboardRenderer> billboardRenderer;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void NextScene(ISceneManager* pSceneManager) override;
};


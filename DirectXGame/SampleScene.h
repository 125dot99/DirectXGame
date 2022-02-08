#pragma once
#include "scene/BaseScene.h"
#include <GameLibHeader.h>

using namespace gamelib;

class SampleScene : public BaseScene
{
	//���C���J����
	std::unique_ptr<Camera> mainCamera;
	std::unique_ptr<Camera> lightCamera;
	//���C�g
	DirectionalLight light;
	std::unique_ptr<LightGroup> lightGroup;
	
	std::vector<std::weak_ptr<IMesh>> models;
	IPipelineState* shader;

	std::vector<GameObject*> objects;
	std::unique_ptr<ObjectManager> objectManager;

	std::unique_ptr<MeshRenderer> meshRenderer;


	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void NextScene(ISceneManager* pSceneManager) override;
};


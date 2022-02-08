#pragma once
#include "scene/BaseScene.h"
#include <GameLibHeader.h>
using namespace gamelib;

class TestScene : public BaseScene
{
private:
	std::unique_ptr<Camera> camera;
	std::unique_ptr<LightGroup> lightGroup;
	DirectionalLight light;

	GameObject* object;
	FbxAnimation* anima;
	std::weak_ptr<IMesh> mesh;
	std::unique_ptr<MeshRenderer> meshRenderer;
	IPipelineState* pipeline;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void NextScene(ISceneManager* pSceneManager) override;
};


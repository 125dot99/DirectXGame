#pragma once
#include <memory>

#include "../object/GameObject.h"
#include "../dx12/IMesh.h"
#include "../dx12/IMaterial.h"
#include "FbxAnimation.h"

namespace gamelib
{
//メッシュの描画クラス
class MeshRenderer
{
private:
	GameObject* pGameObject;
	std::weak_ptr<IMesh> w_pMesh;
	std::weak_ptr<IMaterial> w_pMaterial;
	std::weak_ptr<FbxAnimation> w_pFbxAnimation;
public:
	MeshRenderer();

	MeshRenderer(GameObject* pGameObject, const std::string& _meshName, const std::string& _materialName, std::weak_ptr<FbxAnimation> w_pFbxAnimation = {});
	MeshRenderer(GameObject* pGameObject, std::shared_ptr<IMesh> s_pMesh, std::shared_ptr<IMaterial> s_pMaterial, std::weak_ptr<FbxAnimation> w_pFbxAnimation = {});

	void Draw();
};
} // namespace gamelib
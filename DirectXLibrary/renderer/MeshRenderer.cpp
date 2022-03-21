#include "MeshRenderer.h"

#include "../loader/ResourceManager.h"
#include "../pipeline/PipelineManager.h"

gamelib::MeshRenderer::MeshRenderer(GameObject* pGameObject, const std::string& meshName, const std::string& materialName, std::weak_ptr<FbxAnimation> w_pFbxAnimation)
{
	this->pGameObject = pGameObject;
	this->w_pFbxAnimation = w_pFbxAnimation;
	w_pMesh = ResourceManager::GetInstance()->GetMesh(meshName);
	if (materialName != "")
	{
		w_pMaterial = ResourceManager::GetInstance()->GetMaterial(materialName);
	}
}

gamelib::MeshRenderer::MeshRenderer(GameObject* pGameObject, std::shared_ptr<IMesh> s_pMesh, std::shared_ptr<IMaterial> s_pMaterial, std::weak_ptr<FbxAnimation> w_pFbxAnimation)
{
	this->pGameObject = pGameObject;
	this->w_pFbxAnimation = w_pFbxAnimation;
	w_pMesh = s_pMesh;
	if (s_pMaterial != nullptr)
	{
		w_pMaterial = s_pMaterial;
	}
}

void gamelib::MeshRenderer::Draw()
{
	if (w_pFbxAnimation.lock())
	{
		w_pFbxAnimation.lock()->GraphicsCommand();
	}
	if (w_pMaterial.lock())
	{
		w_pMaterial.lock()->Register();
	}
	pGameObject->GraphicsCommand();
	w_pMesh.lock()->Draw();
}


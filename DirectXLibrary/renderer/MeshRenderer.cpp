#include "MeshRenderer.h"

#include "../loader/ResourceManager.h"
#include "../pipeline/PipelineManager.h"

gamelib::MeshRenderer::MeshRenderer()
{

}

gamelib::MeshRenderer::~MeshRenderer()
{

}

void gamelib::MeshRenderer::Register(const std::string& shaderTag, const std::string& objectName, const std::string& modelName)
{

}

void gamelib::MeshRenderer::Draw(const std::string& shaderTag)
{

}

void gamelib::MeshRenderer::Draw(const GameObject& gameObject, const std::weak_ptr<IMesh>& w_p_mesh, bool isMaterial)
{
	if (!&gameObject || !w_p_mesh.lock())
	{
		return;
	}
	gameObject.GraphicsCommand();
	w_p_mesh.lock()->Draw(isMaterial);
}

void gamelib::MeshRenderer::Draw(const GameObject& gameObject, const std::weak_ptr<IMesh>& w_p_mesh, FbxAnimation* pAnimation, bool isMaterial)
{
	if (pAnimation)
	{
		pAnimation->GraphicsCommand();
	}
	Draw(gameObject, w_p_mesh, isMaterial);
}

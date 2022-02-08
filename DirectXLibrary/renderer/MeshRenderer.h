#pragma once
#include "../object/GameObject.h"
#include "FbxAnimation.h"

namespace gamelib
{
class MeshRenderer
{
public:
	MeshRenderer();
	~MeshRenderer();

	void Register(const std::string& shaderTag, const std::string& objectName, const std::string& modelName);

	void Draw(const std::string& shaderTag);

	void Draw(const GameObject& gameObject, const std::weak_ptr<IMesh>& w_p_mesh, bool isMaterial = true);
	void Draw(const GameObject& gameObject, const std::weak_ptr<IMesh>& w_p_mesh, FbxAnimation* pAnimation, bool isMaterial = true);
	
	//ゲームオブジェクトに付属するコライダーの描画
	//void DrawCollider(const GameObject& gameObject, const std::string _primitive);
};
} // namespace gamelib
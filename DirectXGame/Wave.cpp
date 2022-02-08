#include "Wave.h"

#include "dx12/MeshFactory.h"
#include "posteffect/RenderManager.h"
#include "camera/SceneCameraProvide.h"
#include "pipeline/PipelineManager.h"

#include "imgui/imgui.h"

struct CBTessellationParam
{
	Matrix4 refMat;
	Vector3 factor;
	CBTessellationParam(const Matrix4 refMat, const Vector3& factor) :
		refMat(refMat), factor(factor) {}
};

Matrix4 CalcReflectionMatrix2(const Vector4& normal)
{
	Matrix4 refMat;
	refMat.m[0][0] = 1.0f - 2.0f * normal.x * normal.x;
	refMat.m[0][1] = -2.0f * normal.x * normal.y;
	refMat.m[0][2] = -2.0f * normal.x * normal.z;
	refMat.m[0][3] = -2.0f * normal.x * normal.w;
	refMat.m[1][0] = -2.0f * normal.y * normal.x;
	refMat.m[1][1] = 1.0f - 2.0f * normal.y * normal.y;
	refMat.m[1][2] = -2.0f * normal.y * normal.z;
	refMat.m[1][3] = -2.0f * normal.y * normal.w;
	refMat.m[2][0] = -2.0f * normal.z * normal.x;
	refMat.m[2][1] = -2.0f * normal.z * normal.y;
	refMat.m[2][2] = 1.0f - 2.0f * normal.z * normal.z;
	refMat.m[2][3] = -2.0f * normal.z * normal.w;
	refMat.m[3][0] = refMat.m[3][1] = refMat.m[3][2] = 0.0f;
	refMat.m[3][3] = 1.0f;
	return refMat;
}

Wave::Wave(GameObject* pWaveObject)
{
	isWireFrame = false;
	gameObject = pWaveObject;
	waveMesh.reset(MeshFactory::CreatePatchPlane(16, 1.0f, 1.0f));
	Vector4 normal = Vector4(0, 1, 0, 0);
	refMatrix = CalcReflectionMatrix2(normal);
	tessFactor = Vector3(10.0f, 30.0f, 0);

	tessBuff = std::make_unique<ConstBuffer>();
	tessBuff->Init(3, sizeof(CBTessellationParam));
	CBTessellationParam buff(refMatrix, tessFactor);
	tessBuff->Map(&buff);

	waveShader = PipelineManager::GetInstance()->GetPipelineState("WavePatchShader");
	waveWireShader = PipelineManager::GetInstance()->GetPipelineState("WWavePatchShader");
}

void Wave::Draw()
{
	//ImGui::Begin("Debug");
	//{
	//	ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
	//	if (ImGui::TreeNode("Tessellation"))
	//	{
	//		ImGui::Checkbox("WireFrame", &isWireFrame);
	//		ImGui::SliderFloat("FactorX", &tessFactor.x, 1.0f, 10.0f);
	//		ImGui::SliderFloat("FactorY", &tessFactor.y, 1.0f, 32.0f);
	//		ImGui::SliderFloat("FactorZ", &tessFactor.z, 0.0f, 30.0f);
	//		ImGui::TreePop();
	//	}
	//}
	//ImGui::End();
	CBTessellationParam buff(refMatrix, tessFactor);
	tessBuff->Map(&buff);
	if (isWireFrame)
	{
		waveWireShader->Command();
	}
	else
	{
		waveShader->Command();
	}
	gameObject->GraphicsCommand();
	tessBuff->GraphicsCommand();
	RenderManager::GetInstance()->GetRenderTarget(4)->GraphicsSRVCommand(4);//ハイトマップ
	RenderManager::GetInstance()->GetRenderTarget(5)->GraphicsSRVCommand(5);//法線
	RenderManager::GetInstance()->GetRenderTarget(6)->GraphicsSRVCommand(6);//反射
	waveMesh->Draw(false);
}


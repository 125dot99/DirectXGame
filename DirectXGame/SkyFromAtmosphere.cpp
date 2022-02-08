#include "SkyFromAtmosphere.h"

#include "ConstBuffer.h"
#include "PipelineManager.h"
#include "math/Quaternion.h"
#include "imgui.h"

void SkyFromAtmosphere::Initialize()
{
	//定数バッファの生成
	skyCbBuff.reset(new ConstBuffer);
	skyCbBuff->Init(1, sizeof(SkyCbData));
	
	const float SCALE = 51.0f;
	scale = { SCALE, SCALE, SCALE };
	cb.waveLength = Vector3(0.65f, 0.57f, 0.475f); 
	cb.waveLength.x = 1.0f / powf(cb.waveLength.x, 4.0f);
	cb.waveLength.y = 1.0f / powf(cb.waveLength.y, 4.0f);
	cb.waveLength.z = 1.0f / powf(cb.waveLength.z, 4.0f);
	cb.innerRadius = 50.0f;
	cb.outerRadius = 50.25f;
	cb.eSun = 100.0f;
	cb.scaleDepth = 0.25f;
	cb.kr = 0.0025f;
	cb.km = 0.0010f;
	skyCbBuff->Map(&cb);
}

void SkyFromAtmosphere::Update()
{
	static bool flag = false;
	ImGui::Begin("SkyAtmosphere");
	{
		ImGui::Checkbox("Time", &flag);
		ImGui::SliderFloat("positionY", &position.y, -25.0f, 25.0f);
		ImGui::SliderFloat("ObjectScale", &scale.x, 50.0f, 55.0f);
		ImGui::SliderFloat("OutRadius", &cb.outerRadius, 50.0f, 55.0f);
		ImGui::SliderFloat("InRadius", &cb.innerRadius, 50.0f, 55.0f);
		ImGui::SliderFloat("ESun", &cb.eSun, 10.0f, 30.0f);
		ImGui::SliderFloat("ScaleDepth", &cb.scaleDepth, 0.0f, 1.0f);
		ImGui::SliderFloat("KR", &cb.kr, 0.0f, 0.005f);
		ImGui::SliderFloat("KM", &cb.km, 0.0f, 0.005f);
		scale = { scale.x, scale.x, scale.x };
	}
	ImGui::End();
	skyCbBuff->Map(&cb);

	if (!flag)
	{
		return;
	}
	const float angle = 0.002f;
	const Vector3 offset = mainLight->GetDirection();
	Quaternion q = AngleAxis(-Vector3::Forward(), angle);
	q = quaternion(offset, q);
	mainLight->SetDirection(q.Euler());
}

void SkyFromAtmosphere::Draw()
{
	PipelineManager::GetInstance()->PipelineCommand(ATMOSPHERE);
	ObjectCbCommand();
	skyCbBuff->Command();
	//ライトのルートパラメータ
	static const int LightRootPrame = 2;
	mainLight->Command(LightRootPrame);
	model->DrawMeshCommand();
}

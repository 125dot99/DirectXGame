#include "GameClock.h"

#include "math/Quaternion.h"
#include "math/MyMath.h"
#include "mydebug/ImGuiWrapper.h"

DirectionalLight GameClock::sunLight;
DirectionalLight GameClock::moonLight;
LightGroup* GameClock::llghtGroup;

void GameClock::Initialize()
{
	sunLight.direction = Vector3(1.0f, 0.0f, 0.0f);
	moonLight.direction = Vector3(-1.0f, 0.0f, 0.0f);
}

void GameClock::SetLightGroup(LightGroup* pLlghtGroup)
{
	llghtGroup = pLlghtGroup;
	sunLight.color = Vector4(0);
	moonLight.direction = Vector3(0.01f, 0.7f, 0.7f).Normalize();
	moonLight.color = Vector4(1, 0.85f, 0.6f, 1);
	sunLight.direction = -moonLight.direction;
	pLlghtGroup->SetDirectionalLight(&sunLight, 0);
	pLlghtGroup->SetDirectionalLight(&moonLight, 1);
}

void GameClock::Update()
{
	return;
	//ŽžŠÔ•Ï‰»—Ê
	static float t = 0.01f;
	const Vector3 dir = sunLight.direction;
	Quaternion v = AngleAxis(Vector3(0.0f, -1.0f, 1.0f), t);
	Quaternion q = Quaternion(dir, v);
	Vector3 directon = q.Euler().Normalize();

	//ƒ‰ƒCƒg‚ð‰ñ“]
	sunLight.direction = directon;
	moonLight.direction = -sunLight.direction;
	
	float sunFactor = Clamp(directon.y + 0.25f, 0.0f, 1.0f);
	sunLight.color = Vector4(1 + 0.25f, 1, 1, 1) * sunFactor;
	sunLight.color.x = Min(1.0f, sunLight.color.x);
	float moonFactor = Clamp(moonLight.direction.y, 0.0f, 1.0f);
	moonLight.color = Vector4(1, 0.85f, 0.6f, 1) * moonFactor;

	ImGui::Begin("Debug");
	{
		ImGui::SliderFloat("Time : ", &t, 0, 0.01f);
	}
	ImGui::End();
}


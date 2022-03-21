#include "ReflectCamera.h"

#include "camera/SceneCameraProvide.h"
#include "math/MyMath.h"

#include "imgui/imgui.h"

ReflectCamera::ReflectCamera(const Camera& camera) : Camera(camera)
{
	u_pTransform->up = Vector3::Up();
}

void ReflectCamera::Update()
{
	//���C���̃J�������Q��
	u_pTransform->eye = SceneCameraProvide::GetCamera()->GetEye() * Vector3(1, -1, 1);
	u_pTransform->target = SceneCameraProvide::GetCamera()->GetTarget() * Vector3(1, -1, 1);
	//�r���[�s��
	matView = MatrixLookAtLH(u_pTransform->eye, u_pTransform->target, u_pTransform->up);
	//�r���[�s��ƃv���W�F�N�V�����s�������
	matViewProj = matView * matProjction;
	
	//���˃J�����̏C��
	//static float m0[4]{};
	//static float m1[4]{};
	//static float m2[4]{};
	//static float m3[4]{};
	//ImGui::Begin("ReflectCamera");
	//{
	//	ImGui::SliderFloat4("M0", m0, -1, 1);
	//	ImGui::SliderFloat4("M1", m1, -1, 1);
	//	ImGui::SliderFloat4("M2", m2, -1, 1);
	//	ImGui::SliderFloat4("M3", m3, -1, 1);
	//	for (int i = 0; i < 4; i++)
	//	{
	//		matViewProj.m[0][i] += m0[i];
	//		matViewProj.m[1][i] += m1[i];
	//		matViewProj.m[2][i] += m2[i];
	//		matViewProj.m[3][i] += m3[i];
	//	}
	//}
	//ImGui::End();
}

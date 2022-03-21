#include "Camera.h"

#include "../app/Application.h"
#include "../math/MyMath.h"

#include "CameraStaticMode.h"
#include "SceneCameraProvide.h"

#include "../pipeline/IPipelineState.h"

void gamelib::Camera::TransferBuffer()
{
	u_pConstBuffer->Map(&CBCameraMatrix(
		matView, 
		matProjction,
		matViewProj, 
		MatrixBillboard(matView, false), 
		u_pTransform->eye));
}

gamelib::Camera::Camera()
{
	u_pConstBuffer = std::make_unique<ConstBuffer>();
	u_pConstBuffer->Init((UINT)ROOT_PARAMETER::CAMERA, sizeof(CBCameraMatrix));
	u_pTransform = std::make_unique<CameraTransform>();
	u_pTransform->eye = Vector3(0, 0, -10);
	u_pTransform->target = Vector3::Zero();
	u_pTransform->up = Vector3::Up();
	state = std::make_unique<CameraStaticMode>();
}

gamelib::Camera::Camera(const Camera& other) : Camera()
{
	*u_pTransform = *other.u_pTransform;
	matProjction = other.matProjction;
}

void gamelib::Camera::Initialize(float fovAngle, float nearZ, float farZ)
{
	Vector2 screenSize = Application::GetInstance()->GetWindowSize();
	float aspect = screenSize.x / screenSize.y;
	matProjction = MatrixPerspectiveFovLH(ConvertToRadian(fovAngle), aspect, nearZ, farZ);
	SceneCameraProvide::SetMainCamera(this);
}

void gamelib::Camera::Update()
{
	state->Update();
	//ビュー行列
	matView = MatrixLookAtLH(u_pTransform->eye, u_pTransform->target, u_pTransform->up);
	//ビュー行列とプロジェクション行列を合成
	matViewProj = matView * matProjction;
}

void gamelib::Camera::ChangeCamera(ICameraState* newState)
{
	state.reset(newState);
}

void gamelib::Camera::RegisterCommand()
{ 
	//バッファの転送
	TransferBuffer();
	u_pConstBuffer->GraphicsCommand();
}

gamelib::CameraTransform* gamelib::Camera::GetTransform() const
{
	return u_pTransform.get();
}

const gamelib::Vector3& gamelib::Camera::GetEye() const
{ 
	return u_pTransform->eye; 
}

const gamelib::Vector3& gamelib::Camera::GetTarget() const
{ 
	return u_pTransform->target;
}

const gamelib::Vector3& gamelib::Camera::GetUp() const
{ 
	return u_pTransform->up;
}

const gamelib::Matrix4& gamelib::Camera::GetMatView() const
{ 
	return matView; 
}

const gamelib::Matrix4& gamelib::Camera::GetMatProjection() const
{
	return matProjction;
}

const gamelib::Matrix4& gamelib::Camera::GetMatViewProjection() const
{ 
	return matViewProj; 
}

gamelib::Vector3 gamelib::Camera::GetDirection() const
{ 
	return u_pTransform->Direction();
}


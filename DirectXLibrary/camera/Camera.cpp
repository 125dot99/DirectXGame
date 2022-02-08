#include "Camera.h"

#include "../app/Application.h"
#include "../math/MyMath.h"

#include "CameraStaticMode.h"
#include "SceneCameraProvide.h"

#include "../pipeline/IPipelineState.h"

void gamelib::Camera::TransferBuffer()
{
	cbuffer->Map(&CBCameraMatrix(
		matView, 
		matProjction,
		matViewProj, 
		MatrixBillboard(matView, false), 
		transform->eye));
}

gamelib::Camera::Camera()
{
	cbuffer = std::make_unique<ConstBuffer>();
	cbuffer->Init((UINT)ROOT_PARAMETER::CAMERA, sizeof(CBCameraMatrix));
	transform = std::make_unique<CameraTransform>();
	transform->eye = Vector3(0, 0, -10);
	transform->target = Vector3::Zero();
	transform->up = Vector3::Up();
	state = std::make_unique<CameraStaticMode>();
}

gamelib::Camera::Camera(const Camera& other) : Camera()
{
	*transform = *other.transform;
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
	matView = MatrixLookAtLH(transform->eye, transform->target, transform->up);
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
	cbuffer->GraphicsCommand();
}

gamelib::CameraTransform* gamelib::Camera::GetTransform() const
{
	return transform.get();
}

const gamelib::Vector3& gamelib::Camera::GetEye() const
{ 
	return transform->eye; 
}

const gamelib::Vector3& gamelib::Camera::GetTarget() const
{ 
	return transform->target;
}

const gamelib::Vector3& gamelib::Camera::GetUp() const
{ 
	return transform->up;
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
	return transform->Direction();
}


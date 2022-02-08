#pragma once
#include "ICameraRead.h"
#include "ICameraState.h"
#include "CameraTransform.h"
#include "../dx12/ConstBuffer.h"

namespace gamelib
{
struct CBCameraMatrix
{
	Matrix4 view;
	Matrix4 projection;
	Matrix4 viewProjection;
	Matrix4 billboard;
	Vector3 cameraPos;
	CBCameraMatrix(
		const Matrix4& _view,
		const Matrix4& _projection,
		const Matrix4& _viewProjection,
		const Matrix4& _billboard,
		const Vector3& _cameraPos) :
		view(_view),
		projection(_projection),
		viewProjection(_viewProjection),
		billboard(_billboard),
		cameraPos(_cameraPos) {}
};

class Camera : public ICameraRead
{
private:
	std::unique_ptr<ConstBuffer> cbuffer;
	std::unique_ptr<ICameraState> state;

protected:
	std::unique_ptr<CameraTransform> transform;
	//�r���[�ϊ��s��
	Matrix4 matView;
	Matrix4 matProjction;
	Matrix4 matViewProj;
	virtual void TransferBuffer() final;
public:
	Camera();
	Camera(const Camera& other);
	virtual ~Camera() = default;
	//�J�����������i��p�A�J������O�A�J�������j
	virtual void Initialize(float fovAngle, float nearZ, float farZ) final;
	//�J�����X�V����
	virtual void Update();
	//�J�����̏�ԕύX
	virtual void ChangeCamera(ICameraState* newState) final;
	//�J������GPU�R�}���h
	virtual void RegisterCommand() final;
	//getter
	CameraTransform* GetTransform() const;
	const Vector3& GetEye() const override final;
	const Vector3& GetTarget() const override final;
	const Vector3& GetUp() const override final;
	const Matrix4& GetMatView() const override final;
	const Matrix4& GetMatProjection() const override final;
	const Matrix4& GetMatViewProjection() const override final;
	Vector3 GetDirection() const override final;
};
} // namespace gamelib

#pragma once
#include "../math/Matrix4.h"
#include "../math/Vector.h"
#include "CameraTransform.h"

namespace gamelib
{
//“Ç‚İæ‚èê—pƒJƒƒ‰
class ICameraRead
{
public:
	virtual ~ICameraRead() = default;
	virtual const Vector3& GetEye() const = 0;
	virtual const Vector3& GetTarget() const = 0;
	virtual const Vector3& GetUp() const = 0;
	virtual const Matrix4& GetMatView() const = 0;
	virtual const Matrix4& GetMatProjection() const = 0;
	virtual const Matrix4& GetMatViewProjection() const = 0;
	virtual Vector3 GetDirection() const = 0;
};
} // namespcae gamelib
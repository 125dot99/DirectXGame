#pragma once
namespace gamelib
{
enum class PRIMITIVE_TYPE
{
	BOX2D,
	SPHERE2D,
	BOX,
	SPHERE,
	RAY,
	CAPSULE,
	TRAIANGLE,
	MESH,
};

class BaseCollider abstract
{
private:
	friend class CollisionManager;
	bool isCheck;
	unsigned short attribute;

protected:
	class GameObject* gameObject;

	/// <summary>
	/// �R���C�_�[�̌^�ϊ�
	/// </summary>
	//virtual operator struct PrimitiveType* () const = 0;
public:
	BaseCollider() : gameObject(nullptr), attribute(0), isCheck(true) {}
	virtual ~BaseCollider() = default;

	/// <summary>
	/// �R���C�_�[�̍X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �R���C�_�[�̔���
	/// </summary>
	/// <returns></returns>
	virtual PRIMITIVE_TYPE GetType() const = 0;
};
} // namespace gamelib

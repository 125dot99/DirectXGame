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
	/// コライダーの型変換
	/// </summary>
	//virtual operator struct PrimitiveType* () const = 0;
public:
	BaseCollider() : gameObject(nullptr), attribute(0), isCheck(true) {}
	virtual ~BaseCollider() = default;

	/// <summary>
	/// コライダーの更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// コライダーの判別
	/// </summary>
	/// <returns></returns>
	virtual PRIMITIVE_TYPE GetType() const = 0;
};
} // namespace gamelib

#pragma once
namespace gamelib
{
//状態抽象インタフェース
class IObjectState abstract
{
public:
	~IObjectState() = default;

	virtual void Update() = 0;
};
} // namespace gamelib
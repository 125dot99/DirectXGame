#pragma once
namespace gamelib
{
class ICameraState abstract
{
public:
	~ICameraState() = default;
	
	virtual void Update() = 0;
};
} // namespace gamelib
#pragma once
namespace gamelib
{
//��Ԓ��ۃC���^�t�F�[�X
class IObjectState abstract
{
public:
	~IObjectState() = default;

	virtual void Update() = 0;
};
} // namespace gamelib
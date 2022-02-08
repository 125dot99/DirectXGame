#pragma once
#include <ctime>
namespace gamelib
{
class Timer
{
private:
	//�J�n
	std::clock_t timeBegin{};
	//�I��
	std::clock_t timeEnd{};
public:
	//�J�n����
	inline void Begin()
	{
		timeBegin = std::clock();
	}
	//�I������
	inline void End()
	{
		timeEnd = std::clock();
	}
	//���Ԃ�b�P�ʂŕԂ�
	inline float ElapsedSec() const
	{
		return static_cast<float>(timeEnd - timeBegin) / CLOCKS_PER_SEC;
	}
};
} // namespace gamelib

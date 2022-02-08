#pragma once
#include <ctime>
namespace gamelib
{
class Timer
{
private:
	//開始
	std::clock_t timeBegin{};
	//終了
	std::clock_t timeEnd{};
public:
	//開始時間
	inline void Begin()
	{
		timeBegin = std::clock();
	}
	//終了時間
	inline void End()
	{
		timeEnd = std::clock();
	}
	//時間を秒単位で返す
	inline float ElapsedSec() const
	{
		return static_cast<float>(timeEnd - timeBegin) / CLOCKS_PER_SEC;
	}
};
} // namespace gamelib

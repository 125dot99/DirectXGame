#include "Fps.h"

gamelib::Fps::Fps()
{
	//1秒あたりのパフォーマンスカウント数を格納
	QueryPerformanceFrequency(&count);
}

void gamelib::Fps::Update()
{
	QueryPerformanceCounter(&endTime);
	//掛かった時間を計測
	took = static_cast<double>(endTime.QuadPart - startTime.QuadPart) / static_cast<double>(count.QuadPart);
	if (took < FRAME_RATE)
	{
		DWORD wait = (DWORD)((FRAME_RATE - took) * 1000);
		timeBeginPeriod(1);
		Sleep(wait);
		timeEndPeriod(1);
		took = FRAME_RATE;
	}
	QueryPerformanceCounter(&startTime);
}

const double& gamelib::Fps::GetFrameRate() const
{
	return took;
}

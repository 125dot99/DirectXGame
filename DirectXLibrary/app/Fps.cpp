#include "Fps.h"

gamelib::Fps::Fps()
{
	//1�b������̃p�t�H�[�}���X�J�E���g�����i�[
	QueryPerformanceFrequency(&count);
}

void gamelib::Fps::Update()
{
	QueryPerformanceCounter(&endTime);
	//�|���������Ԃ��v��
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

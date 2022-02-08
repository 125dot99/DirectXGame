#pragma once
#include <Windows.h>
#pragma comment(lib, "winmm.lib")

namespace gamelib
{
//FPS固定
class Fps
{
private:
	//固定するFps値
	static constexpr double FPS = 60;
	//1秒あたりのフレームレート平均値
	static constexpr double FRAME_RATE = 1.0 / FPS;

	double took;
	//カウンタ
	LARGE_INTEGER count{};
	//測定用
	LARGE_INTEGER startTime{};
	//測定用
	LARGE_INTEGER endTime{};
public:
	Fps();

	/// <summary>
	/// フレーム数の更新と待機
	/// </summary>
	void Update();
	
	/// <summary>
	/// フレーム数の取得
	/// </summary>
	/// <returns></returns>
	const double& GetFrameRate() const;
};
} // namespace gamelib
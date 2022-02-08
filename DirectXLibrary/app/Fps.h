#pragma once
#include <Windows.h>
#pragma comment(lib, "winmm.lib")

namespace gamelib
{
//FPS�Œ�
class Fps
{
private:
	//�Œ肷��Fps�l
	static constexpr double FPS = 60;
	//1�b������̃t���[�����[�g���ϒl
	static constexpr double FRAME_RATE = 1.0 / FPS;

	double took;
	//�J�E���^
	LARGE_INTEGER count{};
	//����p
	LARGE_INTEGER startTime{};
	//����p
	LARGE_INTEGER endTime{};
public:
	Fps();

	/// <summary>
	/// �t���[�����̍X�V�Ƒҋ@
	/// </summary>
	void Update();
	
	/// <summary>
	/// �t���[�����̎擾
	/// </summary>
	/// <returns></returns>
	const double& GetFrameRate() const;
};
} // namespace gamelib
#pragma once
#include <xaudio2.h>
#include <string>
#include <memory>

namespace gamelib
{
//wav�f�[�^
struct WavSound
{
	WAVEFORMATEX format;
	UINT32 dataSize;
	std::unique_ptr<BYTE> buffer;
};
//�T�E���h�Ǘ��N���X�Ƃ��Ĉ���
class Sound
{
private:
	//����
	float volume;
	//�\�[�X�{�C�X
	IXAudio2SourceVoice* pSourceVoice;

	std::weak_ptr<WavSound> wavSonud;
public:
	//XAudio2�̏�����
	static void XAudio2Initialize();
	//XAudio2�̏I������
	static void XAudio2Finalize();
	
	Sound(const std::string& filename);
	//�Đ�����߂�
	~Sound();

	//�ǂݍ��ݍĐ�
	void Play(UINT loopCount = XAUDIO2_LOOP_INFINITE);
	//�ꎞ��~
	void Stop();

	void Fade(float start, float end, float frame);

	void SetVolume(float volume = 1.0f);

};
} // namespace gamelib


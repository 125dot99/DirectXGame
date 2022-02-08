#pragma once
#include <xaudio2.h>
#include <string>
#include <memory>

namespace gamelib
{
//wavデータ
struct WavSound
{
	WAVEFORMATEX format;
	UINT32 dataSize;
	std::unique_ptr<BYTE> buffer;
};
//サウンド管理クラスとして扱う
class Sound
{
private:
	//音量
	float volume;
	//ソースボイス
	IXAudio2SourceVoice* pSourceVoice;

	std::weak_ptr<WavSound> wavSonud;
public:
	//XAudio2の初期化
	static void XAudio2Initialize();
	//XAudio2の終了処理
	static void XAudio2Finalize();
	
	Sound(const std::string& filename);
	//再生をやめる
	~Sound();

	//読み込み再生
	void Play(UINT loopCount = XAUDIO2_LOOP_INFINITE);
	//一時停止
	void Stop();

	void Fade(float start, float end, float frame);

	void SetVolume(float volume = 1.0f);

};
} // namespace gamelib


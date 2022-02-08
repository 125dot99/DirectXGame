#include "Sound.h"

#include <assert.h>
#include <wrl.h>

#include "Xaudio2VoiceCallback.h"
#include "../loader/ResourceManager.h"

#pragma comment(lib, "xaudio2.lib")

static Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
static IXAudio2MasteringVoice* masterVoice;
static Xaudio2VoiceCallback voiceCallback;

void gamelib::Sound::XAudio2Initialize()
{
	if (xAudio2.Get() != nullptr)
	{
		return;
	}
	HRESULT result;
	result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(result));

	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));
}

void gamelib::Sound::XAudio2Finalize()
{
	CoUninitialize();
}

gamelib::Sound::Sound(const std::string& filename)
{
	wavSonud = ResourceManager::GetInstance()->GetSound(filename);
}

gamelib::Sound::~Sound()
{
	if (pSourceVoice)
	{
		Stop();
		pSourceVoice->FlushSourceBuffers();
	}
}

void gamelib::Sound::Play(UINT loopCount)
{
	//音声データ再生
	HRESULT result = xAudio2->CreateSourceVoice(&pSourceVoice, &wavSonud.lock()->format, 0, 2.0f, &voiceCallback);
	if (FAILED(result))
	{
		return;
	}
	XAUDIO2_BUFFER buf{};
	buf.Flags = XAUDIO2_END_OF_STREAM;	//バッファ
	buf.AudioBytes = wavSonud.lock()->dataSize; //オーディオデータのサイズ
	buf.pAudioData = wavSonud.lock()->buffer.get(); //オーディオデータのポインタ
	buf.LoopCount = loopCount; //ループする場合はXAUDIO2_LOOP_INFINITE
	//buf.PlayBegin =
	//buf.PlayLength =
	//buf.LoopBegin = wfex.
	//buf.LoopLength = 
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void gamelib::Sound::Stop()
{
	pSourceVoice->Stop();
}

void gamelib::Sound::Fade(float start, float end, float frame)
{

}

void gamelib::Sound::SetVolume(float volume)
{
	if (this->volume == volume)
	{
		return;
	}
	pSourceVoice->SetVolume(volume);
	this->volume = volume;
}

#include "WavLoader.h"

#include <fstream>
#include <assert.h>
#include <wrl.h>

#include "../loader/UtilityString.h"

#pragma comment(lib, "xaudio2.lib")

//FBXのファイルパス
const std::string SOUND_PATH = "Resources/Sound/";

struct Chunk//チャンクヘッダ
{
	char id[4];		//チャンク毎のID
	int32_t size;	//チャンクサイズ
};
struct RiffHeader//ヘッダチャンク
{
	Chunk chunk;	//"RIFF"
	char tyoe[4];	//"WAVE"
};
struct FormatChunk
{
	Chunk chunk;	//"fmt"
	WAVEFORMAT fmt;	//波型フォーマット
};

gamelib::WavSound* gamelib::WavLoader::ReadWaveFile(const std::string& _filename)
{
	//ファイルオープン
	std::ifstream file;
	file.open(utility_string::StrToWchar(SOUND_PATH + _filename), std::ios_base::binary);
	assert(file.is_open());
	//読み込み処理
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	assert(strncmp(riff.chunk.id, "RIFF", 4) == 0);
	FormatChunk format;
	file.read((char*)&format, sizeof(format));
	Chunk data;
	file.read((char*)&data, sizeof(data));
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	file.close();

	WavSound* output = new WavSound;
	output->buffer.reset((BYTE*)pBuffer);

	memcpy(&output->format, &format.fmt, sizeof(format.fmt));
	output->format.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	output->dataSize = (UINT32)data.size;
	return output;
}
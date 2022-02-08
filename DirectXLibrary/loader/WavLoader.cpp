#include "WavLoader.h"

#include <fstream>
#include <assert.h>
#include <wrl.h>

#include "../loader/UtilityString.h"

#pragma comment(lib, "xaudio2.lib")

//FBX�̃t�@�C���p�X
const std::string SOUND_PATH = "Resources/Sound/";

struct Chunk//�`�����N�w�b�_
{
	char id[4];		//�`�����N����ID
	int32_t size;	//�`�����N�T�C�Y
};
struct RiffHeader//�w�b�_�`�����N
{
	Chunk chunk;	//"RIFF"
	char tyoe[4];	//"WAVE"
};
struct FormatChunk
{
	Chunk chunk;	//"fmt"
	WAVEFORMAT fmt;	//�g�^�t�H�[�}�b�g
};

gamelib::WavSound* gamelib::WavLoader::ReadWaveFile(const std::string& _filename)
{
	//�t�@�C���I�[�v��
	std::ifstream file;
	file.open(utility_string::StrToWchar(SOUND_PATH + _filename), std::ios_base::binary);
	assert(file.is_open());
	//�ǂݍ��ݏ���
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
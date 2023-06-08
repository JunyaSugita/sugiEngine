#pragma once
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <map>
#include <string>

#pragma comment(lib,"xaudio2.lib")

struct ChunkHeader {
	char id[4];
	int32_t size;
};

struct RiffHeader {
	ChunkHeader chunk;
	char type[4];
};

struct FormatChunk {
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

struct SoundData {
	//�g�^�t�H�[�}�b�g
	WAVEFORMATEX wfex;
	//�o�b�t�@�̐擪�A�h���X
	BYTE* pBuffer;
	//�o�b�t�@�̃T�C�Y
	size_t bufferSize;
};

class Sound
{
public:
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	void Initialize(const std::string& directoryPath = "Resources/", const std::string& extension  = ".wav");
	void LoadWave(const std::string& filename);
	void Unload(SoundData* soundData);
	void PlayWave(std::string filename);
	void Delete();

private:
	ComPtr<IXAudio2> xAudio2_;
	std::map<std::string, SoundData> soundDatas_;
	//�T�E���h�i�[�f�B���N�g��
	std::string directoryPath_;
	std::string extension_;
};


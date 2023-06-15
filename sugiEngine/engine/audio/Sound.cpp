#include "Sound.h"
#include <assert.h>

using namespace Microsoft::WRL;
using namespace std;

void Sound::Initialize(const std::string& directoryPath, const std::string& extension)
{
	HRESULT result;
	IXAudio2MasteringVoice* masterVoice;

	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	result = xAudio2_->CreateMasteringVoice(&masterVoice);

	directoryPath_ = directoryPath;
	extension_ = extension;
}

void Sound::LoadWave(const std::string& filename)
{
	//重複チェック
	if (soundDatas_.find(filename) != soundDatas_.end()) {
		return;
	};

	std::string filepath = directoryPath_ + filename + extension_;

	//ファイルオープン
	std::ifstream file;
	file.open(filepath, std::ios_base::binary);
	assert(file.is_open());

	//wavデータ読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	FormatChunk format = {};
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	if (strncmp(data.id, "JUNK", 4) == 0) {
		file.seekg(data.size, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}
	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//ファイルを閉じる
	file.close();

	//読み込んだデータを返す
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	soundDatas_.insert(std::make_pair(filename, soundData));
}

void Sound::Unload(SoundData* soundData)
{
	//delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void Sound::PlayWave(std::string filename)
{
	HRESULT result;

	map<string, SoundData>::iterator it = soundDatas_.find(filename);
	assert(it != soundDatas_.end());

	SoundData& soundData = it->second;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void Sound::Finalize()
{
	xAudio2_.Reset();

	soundDatas_.clear();

	//map<string, SoundData>::iterator it = soundDatas_.begin();

	//for (; it != soundDatas_.end(); ++it) {
	//	Unload(&it->second);
	//}
}

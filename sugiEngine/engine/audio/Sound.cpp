#include "Sound.h"
#include <assert.h>

#pragma comment(lib,"xaudio2.lib")


using namespace Microsoft::WRL;
using namespace std;

void Sound::Initialize(const std::string& directoryPath, const std::string& extension)
{
	Finalize();

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
	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void Sound::PlayWave(const string& filename, bool isLoop)
{
	HRESULT result;

	//重複チェック
	if (sourceVoices_.find(filename) != sourceVoices_.end()) {
		return;
	};

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
	if (isLoop) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();


	sourceVoices_.insert(std::make_pair(filename, pSourceVoice));
}

void Sound::RePlayWave(const std::string& filename, bool isLoop)
{
	//重複チェック
	if (sourceVoices_.find(filename) != sourceVoices_.end()) {
		StopWave(filename);
	};

	PlayWave(filename,isLoop);
}

void Sound::TogglePlayWave(const std::string& filename, bool isLoop)
{
	//重複チェック
	if (sourceVoices_.find(filename) != sourceVoices_.end()) {
		StopWave(filename);
		return;
	};
	PlayWave(filename, isLoop);
}

void Sound::StopWave(const string& filename)
{
	HRESULT result;

	map<string, IXAudio2SourceVoice*>::iterator it = sourceVoices_.find(filename);
	assert(it != sourceVoices_.end());

	result = it->second->Stop();
	sourceVoices_.erase(it);
}

void Sound::SetVolume(const string& filename,float vol)
{
	HRESULT result;

	map<string, IXAudio2SourceVoice*>::iterator it = sourceVoices_.find(filename);
	if (it == sourceVoices_.end()) {
		return;
	}

	result = it->second->SetVolume(vol);
}

void Sound::Finalize()
{
	xAudio2_.Reset();
	sourceVoices_.clear();
	soundDatas_.clear();
}

/**
 * @file Sound.h
 * @brief 音
 */

#pragma once
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <map>
#include <string>

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
	//波型フォーマット
	WAVEFORMATEX wfex;
	//バッファの先頭アドレス
	BYTE* pBuffer;
	//バッファのサイズ
	UINT32 bufferSize;
};

class Sound
{
public:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	void Initialize(const std::string& directoryPath = "Resources/", const std::string& extension  = ".wav");
	void LoadWave(const std::string& filename);
	void Unload(SoundData* soundData);
	void PlayWave(const std::string& filename,bool isLoop = false);	//既に再生中なら鳴らさない
	void RePlayWave(const std::string& filename, bool isLoop = false);//既に再生中なら最初から鳴らす
	void TogglePlayWave(const std::string& filename, bool isLoop = false);//既に再生中なら止める
	void StopWave(const std::string& filename);
	void SetVolume(const std::string& filename, float vol);
	void Finalize();

private:
	ComPtr<IXAudio2> xAudio2_;
	std::map<std::string, SoundData> soundDatas_;
	std::map <std::string, IXAudio2SourceVoice*> sourceVoices_;
	//std::map<>
	//サウンド格納ディレクトリ
	std::string directoryPath_;
	std::string extension_;
};


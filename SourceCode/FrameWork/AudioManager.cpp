#include "AudioManager.h"

#include <fstream>
#include <cassert>




AudioManager::AudioManager() {
	Initialize();
}

AudioManager::~AudioManager() {
	Finalize();
}

void  AudioManager::Initialize() {
	HRESULT result;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result) {
		assert(0);
	}

	// マスターボイスを生成
	result = xAudio2_->CreateMasteringVoice(&masterVoice);
	if FAILED(result) {
		assert(0);
	}

}

void AudioManager::Finalize() {

	//XAudio2開放
	xAudio2_.Reset();
	//音声データ開放
	std::map<std::string, SoundData>::iterator it = soundDatas_.begin();
	for (; it != soundDatas_.end(); ++it) {
		Unload(&it->second);
	}
	soundDatas_.clear();
}

void AudioManager::LoadWave(const std::string& filename){
	//重複したら抜ける
	if (soundDatas_.find(filename)!=soundDatas_.end()) {
		return;
	}
	//ディレクトリパスとファイル名を連結してフルパスとして得る
	std::string fullpath = directoryPath_ + filename;

	// ファイルストリーム
	std::ifstream file;
	// Waveファイルを開く
	file.open(fullpath, std::ios_base::binary);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	// ファイルがRIFFかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	// Formatチャンクの読み込み
	FormatChunk format = {};
	//チャンクヘッダーの確認
	file.read((char*)&format,sizeof(ChunkHeader));
	//if (strncmp(format.chunk.id,"fmt",4)!=0) {
	//	assert(0);
	//}
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクの読み込み
	ChunkHeader data={};
	file.read((char*)&data, sizeof(data));
	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		//読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4)!=0) {
		assert(0);
	}

	// Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Waveファイルを閉じる
	file.close();
	
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	//サウンドデータえお連想配列に格納
	soundDatas_.insert(std::make_pair(filename,soundData));
}

void AudioManager::StopWave(const std::string& filename) {
	//HRESULT result;

	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	//読み込んでなければアサート
	assert(it != soundDatas_.end());

	//サウンドデータの参照
	SoundData& soundData = it->second;

	
	// 波形データの再生
	XAUDIO2_VOICE_STATE xa2state;
	soundData.pSourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0) {
		soundData.pSourceVoice->Stop(0);
		soundData.pSourceVoice->DestroyVoice();
	}

}

void AudioManager::PlayWave(const std::string& filename, const float& Volume) {
	HRESULT result;

	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	//読み込んでなければアサート
	assert(it != soundDatas_.end());

	//サウンドデータの参照
	SoundData& soundData = it->second;

	// 波形フォーマットを元にSourceVoiceの生成
	result = xAudio2_->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	
	assert(SUCCEEDED(result));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)soundData.pBuffer;
	buf.pContext = soundData.pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = soundData.bufferSize;
	soundData.pSourceVoice->SetVolume(Volume);
	// 波形データの再生
	result = soundData.pSourceVoice->SubmitSourceBuffer(&buf);
	result = soundData.pSourceVoice->Start(0);



	//if (xaudio2state.BuffersQueued!=0) {
	//	pSourceVoice->Stop(0);
	//	if FAILED(result) {
	//		delete[] pBuffer;
	//		assert(0);
	//		return;
	//	}
	//	pSourceVoice->FlushSourceBuffers();
	//	pSourceVoice->SubmitSourceBuffer(&buf);
	//}
}


void AudioManager::Unload(SoundData* _SoundData) {
	//バッファのメモリを開放	
	delete[] _SoundData->pBuffer;

	_SoundData->pBuffer = 0;
	_SoundData->bufferSize = 0;
	_SoundData->wfex = {};

}


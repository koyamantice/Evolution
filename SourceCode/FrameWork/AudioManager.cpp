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

	// XAudio�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result) {
		assert(0);
	}

	// �}�X�^�[�{�C�X�𐶐�
	result = xAudio2_->CreateMasteringVoice(&masterVoice);
	if FAILED(result) {
		assert(0);
	}

}

void AudioManager::Finalize() {

	//XAudio2�J��
	xAudio2_.Reset();
	//�����f�[�^�J��
	std::map<std::string, SoundData>::iterator it = soundDatas_.begin();
	for (; it != soundDatas_.end(); ++it) {
		Unload(&it->second);
	}
	soundDatas_.clear();
}

void AudioManager::LoadWave(const std::string& filename){
	//�d�������甲����
	if (soundDatas_.find(filename)!=soundDatas_.end()) {
		return;
	}
	//�f�B���N�g���p�X�ƃt�@�C������A�����ăt���p�X�Ƃ��ē���
	std::string fullpath = directoryPath_ + filename;

	// �t�@�C���X�g���[��
	std::ifstream file;
	// Wave�t�@�C�����J��
	file.open(fullpath, std::ios_base::binary);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	// RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// �t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	// �t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	// Format�`�����N�̓ǂݍ���
	FormatChunk format = {};
	//�`�����N�w�b�_�[�̊m�F
	file.read((char*)&format,sizeof(ChunkHeader));
	//if (strncmp(format.chunk.id,"fmt",4)!=0) {
	//	assert(0);
	//}
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Data�`�����N�̓ǂݍ���
	ChunkHeader data={};
	file.read((char*)&data, sizeof(data));
	//JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id, "JUNK", 4) == 0) {
		//�ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		//
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4)!=0) {
		assert(0);
	}

	// Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Wave�t�@�C�������
	file.close();
	
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	//�T�E���h�f�[�^�����A�z�z��Ɋi�[
	soundDatas_.insert(std::make_pair(filename,soundData));
}

void AudioManager::StopWave(const std::string& filename) {
	//HRESULT result;

	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	//�ǂݍ���łȂ���΃A�T�[�g
	assert(it != soundDatas_.end());

	//�T�E���h�f�[�^�̎Q��
	SoundData& soundData = it->second;

	
	// �g�`�f�[�^�̍Đ�
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
	//�ǂݍ���łȂ���΃A�T�[�g
	assert(it != soundDatas_.end());

	//�T�E���h�f�[�^�̎Q��
	SoundData& soundData = it->second;

	// �g�`�t�H�[�}�b�g������SourceVoice�̐���
	result = xAudio2_->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	
	assert(SUCCEEDED(result));

	// �Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)soundData.pBuffer;
	buf.pContext = soundData.pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = soundData.bufferSize;
	soundData.pSourceVoice->SetVolume(Volume);
	// �g�`�f�[�^�̍Đ�
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
	//�o�b�t�@�̃��������J��	
	delete[] _SoundData->pBuffer;

	_SoundData->pBuffer = 0;
	_SoundData->bufferSize = 0;
	_SoundData->wfex = {};

}


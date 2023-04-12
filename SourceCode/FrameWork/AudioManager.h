#pragma once
#include <Windows.h>
#include <wrl.h>

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

#include<cstdint>

#include <dsound.h>

#include <map>
#include <string>
/// <summary>
/// �I�[�f�B�I
/// </summary>
class AudioManager {
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private: // �T�u�N���X
	// �`�����N�w�b�_
	struct ChunkHeader {
		char	id[4]; // �`�����N����ID
		int32_t		size;  // �`�����N�T�C�Y
	};

	// RIFF�w�b�_�`�����N
	struct RiffHeader {
		ChunkHeader	chunk;   // "RIFF"
		char	type[4]; // "WAVE"
	};

	// FMT�`�����N
	struct FormatChunk {
		ChunkHeader	chunk; // "fmt "
		WAVEFORMATEX fmt;   // �g�`�t�H�[�}�b�g
	};

	// �����f�[�^
	struct SoundData {
		//�g�`�t�H�[�}�b�g
		WAVEFORMATEX wfex;

		//�o�b�t�@�̐擪�A�h���X
		BYTE* pBuffer;

		//�o�b�t�@�̃T�C�Y
		unsigned int bufferSize;


		IXAudio2SourceVoice* pSourceVoice;
	};

public: // �����o�֐�

	AudioManager();
	~AudioManager();

	/// <summary>
	/// ���\�[�X���̃T�E���h���T�E���h�f�[�^�ɓǂݍ��݂܂�
	/// </summary>
	/// <param name="filename">���\�[�X�f�[�^</param>
	void LoadWave(const std::string& filename);

	/// <summary>
	/// �T�E���h�f�[�^���̃T�E���h���Đ����܂�
	/// </summary>
	/// <param name="filename">�T�E���h�f�[�^</param>
	///<param name="volume">�{�����[��</param>
	/// <param name="isloop">���[�v���邩</param>
	void PlayWave(const std::string& filename, const float& volume,bool isloop=false);
	
	/// <summary>
	/// �T�E���h���Đ����Ȃ�T�E���h���~���܂�
	/// </summary>
	/// <param name="filename">�T�E���h�f�[�^</param>
	void StopWave(const std::string& filename);
private:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	/// <summary>
	/// �T�E���h�f�[�^�J��
	/// </summary>
	/// <param name="_SoundData">�T�E���h�f�[�^</param>
	void Unload(SoundData* _SoundData);

private: // �����o�ϐ�
	//XAudio2�̃C���X�^���X
	ComPtr<IXAudio2> xAudio2_;
	//�T�E���h�f�[�^�̘A�z�z��
	std::map<std::string, SoundData> soundDatas_;

	IXAudio2MasteringVoice* masterVoice_;
	//�T�E���h�f�[�^�i�[�f�B���N�g��
	const std::string directoryPath_ = "Resources/Sound/";
};


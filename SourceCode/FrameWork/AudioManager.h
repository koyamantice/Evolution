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
/// オーディオ
/// </summary>
class AudioManager {
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private: // サブクラス
	// チャンクヘッダ
	struct ChunkHeader {
		char	id[4]; // チャンク毎のID
		int32_t		size;  // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader {
		ChunkHeader	chunk;   // "RIFF"
		char	type[4]; // "WAVE"
	};

	// FMTチャンク
	struct FormatChunk {
		ChunkHeader	chunk; // "fmt "
		WAVEFORMATEX fmt;   // 波形フォーマット
	};

	// 音声データ
	struct SoundData {
		//波形フォーマット
		WAVEFORMATEX wfex;

		//バッファの先頭アドレス
		BYTE* pBuffer;

		//バッファのサイズ
		unsigned int bufferSize;


		IXAudio2SourceVoice* pSourceVoice;
	};

public: // メンバ関数

	AudioManager();
	~AudioManager();

	/// <summary>
	/// リソース内のサウンドをサウンドデータに読み込みます
	/// </summary>
	/// <param name="filename">リソースデータ</param>
	void LoadWave(const std::string& filename);

	/// <summary>
	/// サウンドデータ内のサウンドを再生します
	/// </summary>
	/// <param name="filename">サウンドデータ</param>
	///<param name="volume">ボリューム</param>
	/// <param name="isloop">ループするか</param>
	void PlayWave(const std::string& filename, const float& volume,bool isloop=false);
	
	/// <summary>
	/// サウンドが再生中ならサウンドを停止します
	/// </summary>
	/// <param name="filename">サウンドデータ</param>
	void StopWave(const std::string& filename);
private:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// サウンドデータ開放
	/// </summary>
	/// <param name="_SoundData">サウンドデータ</param>
	void Unload(SoundData* _SoundData);

private: // メンバ変数
	//XAudio2のインスタンス
	ComPtr<IXAudio2> xAudio2_;
	//サウンドデータの連想配列
	std::map<std::string, SoundData> soundDatas_;

	IXAudio2MasteringVoice* masterVoice_;
	//サウンドデータ格納ディレクトリ
	const std::string directoryPath_ = "Resources/Sound/";
};


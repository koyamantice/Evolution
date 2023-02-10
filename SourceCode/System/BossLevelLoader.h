#pragma once

#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <assert.h>

class BossLevelLoader {
public:
	struct LevelData {
		//ボスの名前
		std::string name;
		//移動速度
		float vel;
		//最初のフェーズ
		std::string firstPhase;
		//クールタイム
		float coolTime;
	
	};
public:
	BossLevelLoader() {};
	~BossLevelLoader();
	void Unload(LevelData* _LevelData);

	// レベルデータの読み込みと再生
	void LoadData(const std::string& _filename);
	//
	LevelData takeData(const std::string& _bossname);

private:
	//レベルデータの連想配列
	std::map<std::string, LevelData> levelDatas_;

	//レベルデータ格納ディレクトリ
	const std::string directoryPath_ = "Resources/csv/";

	//書き取り用ストリング
	std::stringstream writeStrings_;

};


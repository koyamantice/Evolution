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
		//体力
		float hp;
		//移動速度
		float vel;
		//初期スケール
		float scale;
		//クールタイム
		float coolTimeMax;
		//攻撃準備時間
		float predictTimeMax;
		//攻撃時間
		float attackTimeMax;

	};
public:
	BossLevelLoader() {};
	~BossLevelLoader();
	void Unload(LevelData* _LevelData);

	// レベルデータの読み込み
	void LoadData(const std::string& _filename);
	//レベルデータの取得
	LevelData takeData(const std::string& bossname);

private:
	//レベルデータの連想配列
	std::map<std::string, LevelData> levelDatas_;

	//レベルデータ格納ディレクトリ
	const std::string directoryPath_ = "Resources/csv/";

	//書き取り用ストリング
	std::stringstream writeStrings_;

};


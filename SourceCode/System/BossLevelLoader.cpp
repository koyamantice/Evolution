#include "BossLevelLoader.h"




BossLevelLoader::~BossLevelLoader() {
	//音声データ開放
	std::map<std::string, LevelData>::iterator it = levelDatas_.begin();
	for (; it != levelDatas_.end(); ++it) {
		Unload(&it->second);
	}
	levelDatas_.clear();

}

void BossLevelLoader::Unload(LevelData* _LevelData) {

	_LevelData->name = {};
	_LevelData->hp = 0;
	_LevelData->vel = 0;
	_LevelData->scale = 0;
	_LevelData->coolTime = 0;
}

void BossLevelLoader::LoadData(const std::string& _filename) {

	//csvを開けて写します。
	std::string filePath = directoryPath_ + _filename + ".csv";
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());
	writeStrings_ << file.rdbuf();
	file.close();

	std::string line;
	while (getline(writeStrings_, line)) {
		//解析しやすくする
		std::istringstream line_stream(line);
		std::string word;

		//半角スペース区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');
		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//飛ばす
			continue;
		}
		LevelData _leveldata = {};
		//名前タグ
		_leveldata.name = word.c_str();
		getline(line_stream, word, ',');
		//体力
		_leveldata.hp = (float)std::atof(word.c_str());
		getline(line_stream, word, ',');
		//速度
		_leveldata.vel = (float)std::atof(word.c_str());
		getline(line_stream, word, ',');
		//初期スケール
		_leveldata.scale = (float)std::atof(word.c_str());
		getline(line_stream, word, ',');
		//クールタイム
		_leveldata.coolTime = (float)std::atof(word.c_str());

		//レベルデータを連想配列に格納
		levelDatas_.insert(std::make_pair(_leveldata.name, _leveldata));

	}
}

BossLevelLoader::LevelData BossLevelLoader::takeData(const std::string& _bossname) {
	std::map<std::string, LevelData>::iterator it = levelDatas_.find(_bossname);
	//読み込んでなければアサート
	assert(it != levelDatas_.end());

	return it->second;
}

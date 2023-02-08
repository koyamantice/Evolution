#include "BossLevelLoader.h"

BossLevelLoader::BossLevelLoader() {
}

BossLevelLoader::~BossLevelLoader() {
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
		//速度
		_leveldata.vel = (float)std::atof(word.c_str());
		getline(line_stream, word, ',');
		//最初のフェーズ
		_leveldata.firstPhase = word.c_str();
		getline(line_stream, word, ',');
		//クールタイム
		_leveldata.coolTime = (float)std::atof(word.c_str());

		//レベルデータを連想配列に格納
		levelDatas_.insert(std::make_pair(_leveldata.name, _leveldata));
		
		
	}
}

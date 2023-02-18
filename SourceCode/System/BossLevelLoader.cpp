#include "BossLevelLoader.h"




BossLevelLoader::~BossLevelLoader() {
	//�����f�[�^�J��
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

	//csv���J���Ďʂ��܂��B
	std::string filePath = directoryPath_ + _filename + ".csv";
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());
	writeStrings_ << file.rdbuf();
	file.close();

	std::string line;
	while (getline(writeStrings_, line)) {
		//��͂��₷������
		std::istringstream line_stream(line);
		std::string word;

		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		getline(line_stream, word, ',');
		//"//"����n�܂�s�̓R�����g
		if (word.find("//") == 0) {
			//��΂�
			continue;
		}
		LevelData _leveldata = {};
		//���O�^�O
		_leveldata.name = word.c_str();
		getline(line_stream, word, ',');
		//�̗�
		_leveldata.hp = (float)std::atof(word.c_str());
		getline(line_stream, word, ',');
		//���x
		_leveldata.vel = (float)std::atof(word.c_str());
		getline(line_stream, word, ',');
		//�����X�P�[��
		_leveldata.scale = (float)std::atof(word.c_str());
		getline(line_stream, word, ',');
		//�N�[���^�C��
		_leveldata.coolTime = (float)std::atof(word.c_str());

		//���x���f�[�^��A�z�z��Ɋi�[
		levelDatas_.insert(std::make_pair(_leveldata.name, _leveldata));

	}
}

BossLevelLoader::LevelData BossLevelLoader::takeData(const std::string& _bossname) {
	std::map<std::string, LevelData>::iterator it = levelDatas_.find(_bossname);
	//�ǂݍ���łȂ���΃A�T�[�g
	assert(it != levelDatas_.end());

	return it->second;
}

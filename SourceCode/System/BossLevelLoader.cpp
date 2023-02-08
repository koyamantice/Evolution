#include "BossLevelLoader.h"

BossLevelLoader::BossLevelLoader() {
}

BossLevelLoader::~BossLevelLoader() {
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
		//���x
		_leveldata.vel = (float)std::atof(word.c_str());
		getline(line_stream, word, ',');
		//�ŏ��̃t�F�[�Y
		_leveldata.firstPhase = word.c_str();
		getline(line_stream, word, ',');
		//�N�[���^�C��
		_leveldata.coolTime = (float)std::atof(word.c_str());

		//���x���f�[�^��A�z�z��Ɋi�[
		levelDatas_.insert(std::make_pair(_leveldata.name, _leveldata));
		
		
	}
}

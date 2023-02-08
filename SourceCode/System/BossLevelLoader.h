#pragma once

#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <assert.h>

class BossLevelLoader {
public:
	struct LevelData {
		//�{�X�̖��O
		std::string name;
		//�ړ����x
		float vel;
		//�ŏ��̃t�F�[�Y
		std::string firstPhase;
		//�N�[���^�C��
		float coolTime;
	
	};
public:
	BossLevelLoader();
	~BossLevelLoader();

	// ���x���f�[�^�̓ǂݍ��݂ƍĐ�
	void LoadData(const std::string& _filename);

private:
	//���x���f�[�^�̘A�z�z��
	std::map<std::string, LevelData> levelDatas_;

	//���x���f�[�^�i�[�f�B���N�g��
	const std::string directoryPath_ = "Resources/csv/";

	//�������p�X�g�����O
	std::stringstream writeStrings_;

};


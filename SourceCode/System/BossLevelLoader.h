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
		//�̗�
		float hp;
		//�ړ����x
		float vel;
		//�����X�P�[��
		float scale;
		//�N�[���^�C��
		float coolTimeMax;
		//�U����������
		float predictTimeMax;
		//�U������
		float attackTimeMax;

	};
public:
	BossLevelLoader() {};
	~BossLevelLoader();
	void Unload(LevelData* _LevelData);

	// ���x���f�[�^�̓ǂݍ���
	void LoadData(const std::string& _filename);
	//���x���f�[�^�̎擾
	LevelData takeData(const std::string& bossname);

private:
	//���x���f�[�^�̘A�z�z��
	std::map<std::string, LevelData> levelDatas_;

	//���x���f�[�^�i�[�f�B���N�g��
	const std::string directoryPath_ = "Resources/csv/";

	//�������p�X�g�����O
	std::stringstream writeStrings_;

};


#pragma once
#include "SceneManager.h"
#include <Touch.h>
#include <string>
#include <fstream>
#include <iostream>



class BattleScene : public BaseScene {

public:
	virtual ~BattleScene() = default;

protected:
	/// <summary>
	/// �o�g���X�e�[�W��p�̏���������
	/// </summary>
	void BattleInit();
	/// <summary>
	/// �t�B�[���h�̍X�V����
	/// </summary>
	void FieldUpdate();
	/// <summary>
	/// ������csv�ǂݍ���
	/// </summary>
	/// <param name="color">�J���[</param>
	void TorchSetup(int color=0);
	/// <summary>
	/// �����\���p�̍X�V
	/// </summary>
	virtual void DescriptionUpdate() {};
protected:

	std::unique_ptr<PauseUI> pauseUi{};
	std::unique_ptr<Sprite>	screens[2]{};

	std::unique_ptr<Sprite> filter_first{};
	float filter_alpha = 0.0f;

	std::unique_ptr<Object3d> skydome{};
	std::unique_ptr<Object3d> ground{};

	std::stringstream torch_pop;
	std::list<std::unique_ptr<Touch>> torchs;


	//kill�N���b�v
	int finishTime = 0;

	bool Intro = true;
	bool Result = false;
	bool GameOver = false;
};
#pragma once
#include "SceneManager.h"


/// <summary>
/// �{�X�F�L�m�R�@���X�e�[�W
/// </summary>
class FirstStage : public BattleScene {
private:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXCommon* dxCommon) override;
	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update(DirectXCommon* dxCommon) override;
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;
	/// <summary>
	/// /// �I��
	/// </summary>
	void Finalize() override;
private:
	/// <summary>
	/// �����J����
	/// </summary>
	/// <param name="Timer"></param>
	void IntroCamera(const float& Timer);
	
	/// <summary>
	/// �N���A��J����
	/// </summary>
	/// <param name="Timer"></param>
	void ResultCamera(const float& Timer);


	/// <summary>
	/// ���j��J����
	/// </summary>
	/// <param name="Timer"></param>
	void SmashCamera(const float& Timer);



	/// <summary>
	///	���������̍X�V����
	/// </summary>
	/// <returns></returns>
	bool IntroUpdate()override;

	/// <summary>
	/// �����\���p�̍X�V
	/// </summary>
	void DescriptionUpdate() override;

	/// <summary>
	/// �N���A�X�V����
	/// </summary>
	bool ClearUpdate() override;

	/// <summary>
	/// �Q�[���I�[�o�[�̍X�V����
	/// </summary>
	bool GameOverUpdate()override;

	float intro_count = 0;
	const float intro_count_max = 800;

	float intro_speed = 1;
	float intro_speed_max = 2;

	const float first_hight = 50.0f;
	//���������ϐ�
	static const int intro_word_max = 6;
	int nowWord = 0;
	std::unique_ptr<Sprite> IntroWord[intro_word_max];
private:

	PostEffect* postEffect = nullptr;
};


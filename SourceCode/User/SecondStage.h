#pragma once
#include "SceneManager.h"


/// <summary>
/// �{�X�F�I�@���X�e�[�W
/// </summary>
class SecondStage : public  BattleScene {
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
	///	���������̍X�V����
	/// </summary>
	/// <returns></returns>
	bool IntroUpdate()override;

	/// <summary>
	/// �N���A�X�V����
	/// </summary>
	bool ClearUpdate() override;

	/// <summary>
	/// �Q�[���I�[�o�[�̍X�V����
	/// </summary>
	void GameOverUpdate()override;

	float RandHeight(const float& base);


	float intro_count = 0;
	const float intro_count_max = 1200;
	float intro_speed = 1;
	float intro_speed_max = 2;

	const float first_hight = 10;
	const XMFLOAT3 first_target = {0,5,0};

	std::unique_ptr<Sprite> Clear{};
	std::unique_ptr<Sprite> Over{};

	
	PostEffect* postEffect = nullptr;
};


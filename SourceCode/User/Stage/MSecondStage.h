#pragma once
#include "SceneManager.h"

/// <summary>
/// �Z�J���h�X�e�[�W�Ɍ������ۂ̃`���[�g���A��
/// </summary>
class MSecondStage : public  BattleScene {
private:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(DirectXCommon* dxCommon) override;
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update(DirectXCommon* dxCommon) override;
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

private:
	/// <summary>
	///	���������̍X�V����
	/// </summary>
	bool IntroUpdate()override;
	/// <summary>
	///	���������̍X�V����
	/// </summary>
	bool MissionUpdate();

	//�N���A��J����
	void ResultCamera(int timer);
	/// <summary>
/// ���j��J����
/// </summary>
/// <param name="timer"></param>
	void SmashCamera(const float& timer) override;

	/// <summary>
	/// �N���A�X�V����
	/// </summary>
	bool ClearUpdate() override;
	/// <summary>
	/// �n�`�~�c�̏���
	/// </summary>
	void HoneyUpdate();

	void DrawLocal() override;
private:
	enum {
		kLeftNest = 0,
		kMiddleNest,
		kRightNest,
		kMaxNestNum
	};

	Actor* honey_[kMaxNestNum] = {};

	//�~�b�V����
	std::unique_ptr<Sprite> mission_{};


	enum {
		kHoneyNumMax = 4,
	};
	int nowOpenHoney = 0;

	bool isVisible_ = true;
	int visible_timer_ = 0;
	const int kVisibleTimerMax = 180;
	float mission_ease_timer_ = 0;
	const float kMissionEaseTimerMax = 80;


	XMFLOAT2 mission_pos_ = {};
	XMFLOAT2 number_pos_ = {};


	//�I���Q�b�g
	std::unique_ptr<Sprite>	honey_get_[kHoneyNumMax]{};

	int smash_honey_ = 0;
	XMFLOAT3 s_eye = {};
	XMFLOAT3 s_target = {};

	PostEffect* postEffect = nullptr;
};


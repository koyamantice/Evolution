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

	//�N���A��J����
	void ResultCamera(int Timer);
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
		kHoneyNumMax=4,
	};
	int nowOpenHoney = 0;
	//�I���Q�b�g
	std::unique_ptr<Sprite>	honey_get_[kHoneyNumMax]{};

	PostEffect* postEffect = nullptr;
};


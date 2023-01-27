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
	void HoneyUpdate();
private:
	enum {
		kLeftNest = 0,
		kMiddleNest,
		kRightNest,
		kMaxNestNum
	};

	Actor* honey_[kMaxNestNum] = {};

	std::unique_ptr<Sprite> Clear{};
	std::unique_ptr<Sprite> Over{};

	PostEffect* postEffect = nullptr;
};


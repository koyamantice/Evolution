
#pragma once
#include"Boss.h"

class Bullet;

class Hornet :public Boss {
public:
	Hornet() {};
private:
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinalize()override;
	void OnCollision(const std::string& Tag)override;

	void IntroOnUpdate(const float timer)override;

	//�I�[�o�[���C�h�����o�֐�
	void StartAction() override;
	void AttackPredict() override;
	void PressAttack() override;
	void ChasePlayer() override;
	void FeedHoney() override;
	void SpecialPinch();
	//
	bool overDamage = false;
	//���Ԃ�Ȃ��悤�ɒ���
	void HoneyControl();

	float DirRotation(const XMFLOAT3& target);

	enum class E_Motion :int {
		kMotionNone = 0,
		//�v���X�U��
		kFollowPlayer,
		kPressBee,
		kLeaveHome,
		//�ړ��U��
		kFirstMoving,
		kSecondMoving,
		kThirdMoving,
		kFourthMoving,

	};
	//�U���̍s���p�^�[��
	E_Motion motion_ = E_Motion::kMotionNone;
	//�A��������Ȃ���
	bool hit_once_ = false;
	//�C�[�W���O�ϐ�
	XMFLOAT3 before_pos{};
	XMFLOAT3 after_pos{};

	enum {
		kLeftHoney=0,
		kRightHoney,
		kHoneyMax
	};
	Actor* honey[kHoneyMax]{};

	int honey_approch_ = 0;


	float fade_frame_ = 0.0f;


	enum class Animation_Type : int {
		kDownAnimation = 0,
		kPosingAnimiation,
		kFlyAnimation
	};

	int attack_count_ = 0;
};
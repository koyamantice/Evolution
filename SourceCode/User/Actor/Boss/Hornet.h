
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

	//オーバーライドメンバ関数
	void StartAction() override;
	void AttackPredict() override;
	void PressAttack() override;
	void ChasePlayer() override;
	void FeedHoney() override;
	void SpecialPinch();
	//
	bool overDamage = false;
	//かぶらないように調整
	void HoneyControl();

	float DirRotation(const XMFLOAT3& target);

	enum class E_Motion :int {
		kMotionNone = 0,
		//プレス攻撃
		kFollowPlayer,
		kPressBee,
		kLeaveHome,
		//移動攻撃
		kFirstMoving,
		kSecondMoving,
		kThirdMoving,
		kFourthMoving,

	};
	//攻撃の行動パターン
	E_Motion motion_ = E_Motion::kMotionNone;
	//連続判定をなくす
	bool hit_once_ = false;
	//イージング変数
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
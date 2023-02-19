
#pragma once
#include"Boss.h"

class Bullet;

class Hornet :public Boss {
public:
	Hornet() {};
	Actor* player;
private:
	void OnInit()override;
	void OnUpda()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinal()override;
	void OnCollision(const std::string&Tag)override;

	void IntroOnUpdate(const float& Timer)override;

	//オーバーライドメンバ関数
	void StartAction() override;
	void AttackPredict() override;
	void PressAttack() override;
	void ChasePlayer() override;
	void FeedHoney() override;

	//
	bool overDamage = false;
	float pinchLife = 20;
	//かぶらないように調整
	void HoneyControl();

	float DirRotation(const XMFLOAT3& target);
	enum class E_Motion :int {
		kMotionNone=0,
		kFollowPlayer,
		kPressBee,
		kLeaveHome,
		//
		kFirstMoving,
		kSecondMoving,
		kThirdMoving,
		kFourthMoving,

	};


	E_Motion motion_ = E_Motion::kMotionNone;
	bool OnePunch = false;

	XMFLOAT3 before_pos{};
	XMFLOAT3 after_pos{};
	float speed = 0.2f;
	const float accel = speed / 30.0f;

	Actor* honey[2]{};
	int honey_approch_ = 0;

	float commandTimer = 0;
	float vel = 0;
	float scale = 0.01f;
	float scaframe = 0;

	XMFLOAT3 h_pos{};
	float time_f =0;
	float time_e = 200;
	float dead_rot_ = 0;
	enum {
		Dawn=0,
		Posing,
		Fly
	};

	bool first_pose=false;
	int rand_pattern = 0;
	int countAttack = 0;
	int  waiting_timer = 0;
	int  change_time = 30;
};
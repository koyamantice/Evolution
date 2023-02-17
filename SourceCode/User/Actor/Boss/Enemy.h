#pragma once
#include"Boss.h"

#include "Player.h"
#include"EnemyUI.h"
#include"EnemyAttack.h"


class Bullet;

class Enemy :public Boss {
public:
	Enemy(){};

protected:
	void OnInit()override;
	void OnUpda()override;
	void OnFirstDraw(DirectXCommon* dxCommon)override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnLastDraw(DirectXCommon* dxCommon)override;
	void OnFinal()override;
	//フェーズまとめ
	void PhaseMove();
	void UnguardUpda();
	void AttackUpda();
	
	//
	void LifeCommon();
	void ChangeCommand(const int& num = 0, const int& command = 0, const int& count = 1);

	void OnCollision(const std::string& Tag);

	std::unique_ptr<EnemyAttack> Attack;

	float speed = 0.2f;
	const float accel = speed / 30.0f;

	int animecount = 0;
	bool standby = false;

	float waitTimer = 0;
	const float kPredictTime = 240;
	const float kScaleCount = 4.0f;
	const float kAttackTime = 150;
	float vel = 0;
	float scale = 0.01f;
	float scaframe = 0;

};
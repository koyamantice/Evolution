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

	//オーバーライドメンバ関数
	void StartAction() override;
	void AttackPredict() override;
	void PressAttack() override;

	void OnCollision(const std::string& Tag);

	std::unique_ptr<EnemyAttack> attack_;

	float speed = 0.2f;
	const float accel = speed / 30.0f;
};
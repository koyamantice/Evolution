#pragma once
#include"Boss.h"

class Bullet;

class CowKing :public Boss {
public:
	CowKing(){};

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
	void ChasePlayer() override;

	float DirRotation(const XMFLOAT3& target);
	std::unique_ptr<EnemyAttack> attack_;

	Actor* player_ = nullptr;
	float speed = 0.2f;
	const float accel = speed / 30.0f;
};
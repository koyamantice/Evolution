#pragma once
#include"Boss.h"

class Bullet;

class MashGhost :public Boss {
public:
	MashGhost(){};

protected:
	void OnInitialize()override;
	void OnUpdate()override;
	void OnFirstDraw(DirectXCommon* dxCommon)override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnLastDraw(DirectXCommon* dxCommon)override;
	void OnFinalize()override;
	void OnCollision(const std::string& Tag)override;

	//オーバーライドメンバ関数
	void StartAction() override;
	void AttackPredict() override;
	void JumpAttack() override;
	void PressAttack() override;
	void StopMotion() override;
	void SpecialPinch() override;
	std::unique_ptr<EnemyAttack> attack_;
	int odd_ = 1;
	int stamp_count_ = 0;
	float speed = 0.2f;
	const float accel = speed / 30.0f;
};
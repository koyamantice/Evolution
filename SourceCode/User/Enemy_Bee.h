
#pragma once
#include"Actor.h"
#include"Input.h"
#include<list>
#include <sstream>
#include <SourceCode/Obj/2d/Object2d.h>
#include "Player.h"
#include"EnemyUI.h"
#include"EnemyAttack.h"

class Bullet;

class Enemy_Bee :public Actor {
public:
	Enemy_Bee() {};
	void LoadData();
	void UpdateCommand();
	void DebugUpdate();
private:
	Input* input = Input::GetInstance();
	std::stringstream parameterCommands;

	Actor* player;

	void OnInit()override;
	void OnUpda()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinal()override;
	void OnCollision(const std::string& Tag)override;
	//フェーズまとめ
	void PhaseMove();
	void UnguardUpda();
	void LeaveUpda();
	void WaitUpda();
	void AttackUpda();
	//
	void LifeCommon();
	//かぶらないように調整
	void HoneyControl();



	std::unique_ptr<EnemyAttack> Attack;
	//
	void ChangeCommand(const int& num = 0,const int& command = 0,const int& count = 1,const bool& reverese=false);

	int pattern = 0;
	bool OnePunch = false;
	XMFLOAT3 before_pos{};
	XMFLOAT3 after_pos{};
	float speed = 0.2f;
	const float accel = speed / 30.0f;
	std::unique_ptr<Object2d> Shadow = nullptr;
	Actor* honey[2]{};

	float waitTimer = 0;
	float vel = 0;
	float scale = 0.01f;
	float scaframe = 0;
	std::unique_ptr<FBXObject3d> fbxObject3d;





	int rand_pattern = 0;
	int  waiting_timer = 0;
	int  change_time = 30;
	bool WaitingElapsedTime();
};
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

class Enemy :public Actor {
public:
	Enemy(){};
	void LoadData();
	void UpdateCommand();
	void DebugUpdate();
protected:
	Input* input =Input::GetInstance();
	std::stringstream parameterCommands;

	Actor* player;

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
	std::unique_ptr<Object2d> Shadow = nullptr;


	float waitTimer = 0;
	float vel = 0;
	float scale = 0.01f;
	float scaframe = 0;
	std::unique_ptr<FBXObject3d> fbxObject3d;

	ParticleManager* partMan = nullptr;
};
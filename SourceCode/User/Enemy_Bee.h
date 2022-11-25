
#pragma once
#include"Actor.h"
#include"Input.h"
#include<list>
#include <sstream>
#include <SourceCode/Obj/2d/Texture.h>
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
	//フェーズまとめ
	void PhaseMove();
	void ApprochUpda();
	void LeaveUpda();
	void WaitUpda();
	void AttackUpda();
	//
	void LifeCommon();
	std::unique_ptr<EnemyAttack> Attack;


	float speed = 0.2f;
	const float accel = speed / 30.0f;
	std::unique_ptr<Texture> Shadow = nullptr;


	float waitTimer = 0;
	float vel = 0;
	float scale = 0.01f;
	float scaframe = 0;
	std::unique_ptr<FBXObject3d> Mash;

};
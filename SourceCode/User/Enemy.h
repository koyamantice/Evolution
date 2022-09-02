#pragma once
#include"Actor.h"
#include"Input.h"
#include<list>
#include <sstream>
#include <SourceCode/Obj/2d/Texture.h>
#include "Player.h"
#include "FBXObject3d.h"
class Bullet;

class Enemy :public Actor {
	enum class Phase{
		Approch,
		Leave,
		Wait,
	};
public:
	Enemy(){};
	void LoadData();
	void UpdateCommand();
	void DebugUpdate();
private:
	Input* input =Input::GetInstance();
	std::stringstream parameterCommands;

	Actor* player;

	void OnInit()override;
	void OnUpda()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinal()override;
	void Collide();
	//フェーズまとめ
	void PhaseMove();
	void ApprochUpdate();
	void LeaveUpdate();
	void WaitUpdate();
	//
	void LifeCommon();

	Phase phase_= Enemy::Phase::Approch;
	float waitTimer = 0;
	float vel = 0;
	std::unique_ptr<FBXObject3d> Mash;
};
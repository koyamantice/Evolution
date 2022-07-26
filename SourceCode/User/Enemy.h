#pragma once
#include"Actor.h"
#include"Input.h"
#include<list>
#include <sstream>
#include <SourceCode/Obj/2d/Texture.h>
#include "Player.h"
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
	void OnDraw()override;
	void OnFinal()override;

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
};
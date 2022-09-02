#pragma once
#include"Actor.h"
#include"Input.h"
#include<list>
#include <sstream>
#include <SourceCode/Obj/2d/Texture.h>
#include"Player.h"


class Crystal :public Actor {
	enum class Phase {
		Approch,
		Leave,
		Wait,
	};
public:
	Crystal() {};
	//void LoadData();
	//void UpdateCommand();
	//void DebugUpdate();
private:
	Input* input = Input::GetInstance();
	std::stringstream parameterCommands;


	void OnInit()override;
	void OnUpda()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinal()override;

	//フェーズまとめ
	void PhaseMove();
	//void ApprochUpdate();
	//void LeaveUpdate();
	//void WaitUpdate();
	////
	//void LifeCommon();

	Actor* player;
	Actor* enemy;



	float waitTimer = 0;
	float vel = 0;
};
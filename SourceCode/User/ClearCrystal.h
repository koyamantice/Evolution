#pragma once
#include"Actor.h"
#include"Input.h"
#include<list>
#include <sstream>
#include <SourceCode/Obj/2d/Object2d.h>
#include"Player.h"


class ClearCrystal :public Actor {
	enum class Phase {
		Approch,
		Leave,
		Wait,
	};
public:
	ClearCrystal() {};
private:
	Input* input = Input::GetInstance();
	std::stringstream parameterCommands;

	void OnInit()override;
	void OnUpda()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinal()override;
	void ResultOnUpdate(const int& Timer)override;
	void OnCollision(const std::string& Tag)override;
	Actor* player=nullptr;

	std::unique_ptr<Object2d> in_out = nullptr;
	bool isCollide = false;
	float frame = 0.0f;
	float a = 0.02f;
	float alpha = 0.0f;
	float hight = 0.0f;

	float rad = 0;
	float waitTimer = 0;
	float vel = 0;
};
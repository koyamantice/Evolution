#pragma once
#include"Actor.h"
#include"Input.h"
#include<list>
#include <sstream>
#include <SourceCode/Obj/2d/Object2d.h>
#include"Player.h"


class Crystal :public Actor {
	enum class Phase {
		Approch,
		Leave,
		Wait,
	};
public:
	Crystal() {};
private:
	Input* input = Input::GetInstance();
	std::stringstream parameterCommands;

	void OnInit()override;
	void OnUpda()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinal()override;
	void OnCollision(const std::string& Tag)override;
	void ResultOnUpdate(const float& Timer)override;
	Actor* player=nullptr;

	std::unique_ptr<Object2d> in_out = nullptr;
	std::unique_ptr<Sprite> Test = nullptr;

	bool isCollide = false;
	float rad = 0;
	float waitTimer = 0;
	float vel = 0;
};
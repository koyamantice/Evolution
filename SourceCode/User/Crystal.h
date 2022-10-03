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
private:
	Input* input = Input::GetInstance();
	std::stringstream parameterCommands;

	void OnInit()override;
	void OnUpda()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinal()override;

	Actor* player=nullptr;

	//std::unique_ptr<Texture>


	float waitTimer = 0;
	float vel = 0;
};
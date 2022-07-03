#pragma once
#include"Actor.h"
#include"Input.h"
class Player :public Actor {
public:
	Player(){};
	void OnInit()override;
	void OnUpda()override;
	void OnDraw()override;
	void OnFinal()override;
private:
	Input* input =Input::GetInstance();

};
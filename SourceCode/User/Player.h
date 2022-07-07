#pragma once
#include"Actor.h"
#include"Input.h"
#include<list>
#include"Bullet.h"

class Player :public Actor {
public:
	Player(){};

	void DebugUpdate();
private:
	Input* input =Input::GetInstance();
	std::list<std::unique_ptr<Bullet>> bullets;
	void OnInit()override;
	void OnUpda()override;
	void OnDraw()override;
	void OnFinal()override;
	void Move();
	void Shot();
};
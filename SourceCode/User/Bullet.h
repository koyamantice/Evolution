#pragma once
#include"Actor.h"
class Player;
class Bullet : public Actor{
public:
	Bullet();
	~Bullet(){};

private:
	void OnInit()override;
	void OnUpda()override;
	void OnDraw()override;
	void OnFinal()override;
};

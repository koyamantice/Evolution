#pragma once
#include"Actor.h"
#include <Bullet.h>

class Honey :public Actor {
public:
	Honey() {};
private:
	int oldStock = 0;
	int More = 0;
	Bullet* Driver[5]{};
private:
	void OnInit()override;
	void OnUpda()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinal()override;

	void ApprochUpda();
	void LeaveUpda();
	void WaitUpda();
	void AttackUpda();
};


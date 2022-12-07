#pragma once
#include"Actor.h"

class Honey :public Actor {
public:
	Honey() {};

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


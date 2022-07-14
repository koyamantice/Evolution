#pragma once
#include"Actor.h"
class Player;
class Bullet : public Actor {
public:
	Bullet();
	~Bullet() {};
	void SetLanding(XMFLOAT3 landing) { this->landing = landing; }
private:
	void OnInit()override;
	void OnUpda()override;
	void OnDraw()override;
	void OnFinal()override;

	bool ease = false;
	float frame = 0.0f;

	float vel = 0.4f;

	XMFLOAT3 landing{};
};

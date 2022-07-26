#pragma once
#include"Actor.h"
#include"Enemy.h"
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
	void Follow();

	bool ease = false;
	float frame = 0.0f;

	float vel = 0.4f;
	std::unique_ptr<Texture> Status{};

	Actor* enemy = nullptr;
	Actor* player = nullptr;
	XMFLOAT3 landing{};
	XMFLOAT2 vel_follow{};
};

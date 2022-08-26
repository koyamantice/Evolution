#pragma once
#include"Actor.h"
#include"Enemy.h"
class Player;

class Bullet : public Actor {
private:
	enum class Pattern {
		Approch,
		Leave,
		Wait,
	};
public:
	Bullet();
	~Bullet() {};
	void SetLanding(XMFLOAT3 landing) { this->landing = landing; }
private:
	void OnInit()override;
	void OnUpda()override;
	void OnDraw()override;
	void OnFinal()override;
	void OnCollision(const std::string& Tag) override;
	
	void WaitUpda();
	void AttackUpda();
	void Follow2Enemy();
	void Follow2Player();

	bool throwReady = false;
	float frame = 0.0f;

	int CoolTime = 0;

	float vel = 0.8f;
	std::unique_ptr<Texture> Status{};

	bool follow = false;
	Actor* enemy = nullptr;
	Actor* player = nullptr;
	XMFLOAT3 landing{};
	XMFLOAT2 vel_follow{};
};

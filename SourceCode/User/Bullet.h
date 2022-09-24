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
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinal()override;
	void OnCollision(const std::string& Tag) override;
	
	void WaitUpda();
	void AttackUpda();
	void Follow2Enemy();
	void Follow2Player();

	void KnockBack();
	bool knockBacking = false;
	float back;
	float Normalize(const XMFLOAT3& pos,const XMFLOAT3& pos2);
	float fall=0.0f;

	bool throwReady = false;
	float frame = 0.0f;

	int CoolTime = 0;

	float vel = 0.8f;
	std::unique_ptr<Texture> Status{};
	std::unique_ptr<Texture> Explo = nullptr;
	bool burning = false;
	void BurnOut();
	float scale = 0.0f;
	float effectRate = 0.0f;
	bool follow = false;
	Actor* enemy = nullptr;
	Actor* player = nullptr;
	XMFLOAT3 landing{};
	XMFLOAT2 vel_follow{};
};

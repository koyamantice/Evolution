#pragma once
#include"Bullet.h"

class BulletRed : public Bullet{
public:
	BulletRed();
	~BulletRed() {};
	void SetLanding(XMFLOAT3 landing) { this->landing = landing; }


private:
	void OnInit()override;
	void OnUpda()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinal()override;
	void OnCollision(const std::string& Tag) override;
	
	void WaitUpda();
	void SlowUpda();
	void AttackUpda();
	void Follow2Enemy();
	void Follow2Player();
	void WaitBullet();

	void KnockBack();
	bool knockBacking = false;
	void DamageInit();
	XMFLOAT3 rebound{};
	XMFLOAT3 distance{};
	XMFLOAT3 exploPos{};
	float damageframe = 0.0f;
	float fall=0.4f;
	bool isLeft = false;
	bool throwReady = false;
	float frame = 0.0f;


	void Dead();
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
	XMFLOAT2 vel_follow{};
};

#pragma once
#include"Bullet.h"

class BulletRed : public Bullet{
public:
	BulletRed();
	~BulletRed() {};
private:
	void OnInit()override;
	//void OnUpda()override;
	//void OnDraw(DirectXCommon* dxCommon)override;
	//void OnFinal()override;
	void ResultOnUpdate(const float& Timer) override;
	void BulletCollision(const XMFLOAT3& pos, const int& Id) override;

	float rad = 0.0f;
	float randRad = 0.0f;
};

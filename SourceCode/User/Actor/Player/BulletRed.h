#pragma once
#include"Bullet.h"

class BulletRed : public Bullet{
public:
	BulletRed();
	~BulletRed() {};
private:
	void OnInitialize()override;
	//void OnUpdate()override;
	//void OnDraw(DirectXCommon* dxCommon)override;
	//void OnFinalize()override;
	void ResultOnUpdate(const float& timer) override;
	void BulletCollision(const XMFLOAT3& pos, const int& Id) override;

	float rad = 0.0f;
	float randRad = 0.0f;
};

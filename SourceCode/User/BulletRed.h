#pragma once
#include"Bullet.h"

class BulletRed : public Bullet{
public:
	BulletRed();
	~BulletRed() {};
private:
	void OnInit()override;
	void OnUpda()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinal()override;
	void OnCollision(const std::string& Tag) override;
};

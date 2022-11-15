#pragma once
#include"Bullet.h"

class BulletGreen : public Bullet{
public:
	BulletGreen();
	~BulletGreen() {};
private:
	void OnInit()override;
	void OnUpda()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinal()override;
	void OnCollision(const std::string& Tag) override;
};

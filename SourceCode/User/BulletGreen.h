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
	void BulletCollision(const XMFLOAT3& pos,const int& Id) override;

	
	void ResultOnUpdate(const int& Timer) override;
};

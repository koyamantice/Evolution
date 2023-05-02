#pragma once
#include"Bullet.h"

class BulletGreen : public Bullet{
public:
	BulletGreen();
	~BulletGreen() {};
private:
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinalize()override;
	void BulletCollision(const XMFLOAT3& pos,const int Id) override;
	void VanishCommand() override;

	
	void ResultOnUpdate(const float timer) override;
};

#pragma once
#include"Actor.h"
#include <Bullet.h>

class SmallMash :public Actor {
public:
	SmallMash() {};
private:
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinalize()override;

	void OnCollision(const std::string& Tag) override;
	float RandHeight(const float& base);

private:
	std::unique_ptr <ParticleEmitter> particleEmitter_ = nullptr;

	Bullet* driver_{};

	XMFLOAT3 before_pos_{};
	XMFLOAT3 after_pos_{};
	XMFLOAT3 first_pos_{};

	float frame = 0;
	int leave_timer_ = 0;
	const int kLeaveTimeMax = 60;

	int particle_draw_num_ = 0;
	const int kDrawTiming = 30;

	//関数ポインタ
	static void(SmallMash::* updateFuncTable[])();

	//メンバ関数
	void RandSpawn();
	void WaitDriver();
	void ParasiteDriver();
	void LapseMash();

	enum class Phase : int {
		kRandSpawn = 0,
		kWaitDriver,
		kParasiteDriver,
		kLapseMash,
	};
	//どの行動を取るか
	Phase phase_ = Phase::kRandSpawn;
};
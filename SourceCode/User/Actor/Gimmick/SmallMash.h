#pragma once
#include"Actor.h"
#include <Bullet.h>

class SmallMash :public Actor {
public:
	SmallMash() {};
private:
	Bullet* driver_{};

	XMFLOAT3 before_pos_{};
	XMFLOAT3 after_pos_{};
	XMFLOAT3 first_pos_{};

	float frame = 0;
	int leave_timer_= 0;
	const int kLeaveTimeMax = 60;
	
	int particle_draw_num_ = 0;		
	const int kDrawTiming = 30;

	std::unique_ptr <ParticleEmitter> particleEmitter_ = nullptr;


	std::unique_ptr<Object2d> missions[2][6]{};
	std::unique_ptr<Object2d> slash{};
private:
	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinalize()override;

	void OnCollision(const std::string& Tag) override;

	void missionUpdate();
	void questionUpdate();


	//関数ポインタ
	static void(SmallMash::* updateFuncTable[])();

	//メンバ関数

	void WaitDriver();
	void DispersionDriver();
	void SetupSmallMash();
	void InviteBee();
	void EatenSmallMash();
	void RandSpawn();

	enum class E_Phase : int {
		kRandSpawn = 0,
		kParasiteDriver,
		kDispersionDriver,
		kSetupSmallMash,
		kInviteBee,
		kEatenSmallMash,
	};
	//どの行動を取るか
	E_Phase phase_ = E_Phase::kParasiteDriver;


	float RandHeight(const float& base);
};


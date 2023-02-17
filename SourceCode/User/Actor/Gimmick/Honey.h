#pragma once
#include"Actor.h"
#include <Bullet.h>

class Honey :public Actor {
public:
	Honey() {};
private:
	int old_stock = 0;
	int ride_num = 0;
	static const int kBulletDriverMax = 5;
	Bullet* driver[kBulletDriverMax]{};

	XMFLOAT3 before_pos{};
	XMFLOAT3 after_pos{};
	XMFLOAT3 first_pos{};

	float frame = 0;

	int leave_timer_= 0;
	const int kLeaveTimeMax = 60;
	
	int particle_draw_num_ = 0;		
	const int kDrawTiming = 30;
	
	const XMFLOAT3 base_sca = { 6,4,6 };

	float scale_damaged_frame_ = 0.0f;
	const float kScaleFrameMax = 1000.0f;
	float scale_damage_ =1.0f/ kScaleFrameMax;
	std::unique_ptr <ParticleEmitter> particleEmitter = nullptr;

	std::unique_ptr<Object3d> honey_obj_{};

	std::unique_ptr<Object2d> missions[2][6]{};
	std::unique_ptr<Object2d> slash{};
private:
	void OnInit()override;
	void OnUpda()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinal()override;

	void OnCollision(const std::string& Tag) override;

	void missionUpdate();
	void questionUpdate();

	void ApprochUpda();
	void LeaveUpda();
	void WaitUpda();
	void DeadUpda();
	void IntroOnUpdate(const float& Timer)override;

	float RandHeight(const float& base);
};


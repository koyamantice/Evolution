#pragma once
#include"Actor.h"
#include <Bullet.h>

class Honey :public Actor {
public:
	Honey() {};
private:
	int old_stock = 0;
	int ride_num = 0;
	Bullet* driver[5]{};
	XMFLOAT3 before_pos{};
	XMFLOAT3 after_pos{};
	XMFLOAT3 first_pos{};
	float frame = 0;
	float unload_frame = 0;

	XMFLOAT3 base_sca = { 4,4,4 };
	XMFLOAT3 question_sca = { 5,5,5 };

	std::unique_ptr<Object3d> questionItem;

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

};


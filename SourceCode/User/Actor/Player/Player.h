#pragma once
#include"Actor.h"
#include"Input.h"
#include<list>
#include"Bullet.h"
#include <sstream>
#include <SourceCode/Obj/2d/Object2d.h>
#include<DirectXMath.h>
#include"Aim.h"
#include <ParticleEmitter.h>

class Player :public Actor {
public:
	Player(){};
	void LoadData();
	void UpdateCommand();
	const XMFLOAT3& GetLockPos()override { return rockpos;}
private:
	Input* input =Input::GetInstance();
	std::stringstream parameterCommands;

	std::unique_ptr<Aim> LockOn{};
	std::unique_ptr<Object2d> Shadow = nullptr;

	XMFLOAT3 rockpos = {0.0f,0.5f,0.0f};

	void OnInit()override;
	void OnUpda()override;
	void OnFirstDraw(DirectXCommon* dxCommon)override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnLastDraw(DirectXCommon* dxCommon)override;
	void OnFinal()override;

	void IntroOnUpdate(const float& Timer)override;
	void IntroMove();
	void ResultOnUpdate(const float& Timer)override;

	void OnCollision(const std::string& Tag)override;
	
	void ShadowUpda();
	void TraceUpda();

	void HitBoundMotion();

	void LimitArea();

	void Move();

	XMFLOAT3 MoveVECTOR(DirectX::XMVECTOR v, float angle);
	DirectX::XMFLOAT3 GetCameraPos(const float& angle, const float& str = 10) override;
	XMVECTOR move_ = { };


	XMFLOAT3 s_rebound_pos_{};
	XMFLOAT3 e_rebound_pos_{};
	const float kKnockBackRange = 15.5f;

	int  aftImage_count_ = 0;
	const int kAftLocateCountMax = 4;

	float knock_back_frame_ = 0.0f;
	const float kKnockBackFrameMax = 50.0f;
	float collided_rot_ = 0.0f;

	bool knockBacking = false;

	float shadow_side_ = 0.2f;

	float y_add = 0.0f;
	int jump_count = 0;

	bool isFasted = false;

	bool onHoney = false;
	int honeyCount = 0;

	float speed = 0.0f;
	float vel = 0;

	int charge = 0;
	
	int particle_pop_time_ = 0;
	const int kPopTimeMax = 5;

	std::unique_ptr<FBXObject3d> fbxObj;
	std::unique_ptr<ParticleEmitter> particleEmitter_ = nullptr;
	std::list<std::unique_ptr<Trace>> traces_;
	int foot_count_ = 0;
	int odd_count_ = 0;
	float foot_rot_ = 0.0f;
};
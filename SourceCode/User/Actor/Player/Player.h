#pragma once
#include"Actor.h"
#include"Aim.h"
#include"Bullet.h"
#include "Object2d.h"
#include"Input.h"
#include <ParticleEmitter.h>

#include<list>
#include <sstream>
#include<DirectXMath.h>

class Player :public Actor {
public:
	Player() {};
	void LoadData();
	void UpdateCommand();
	const XMFLOAT3& GetLockPos()override { return aim_pos_; }
private:
	Input* input = Input::GetInstance();
	//パラメーターコマンド
	std::stringstream parameterCommands;
	//仮想オーバーロード関数
	void OnInitialize()override;
	void OnUpdate()override;
	void OnFirstDraw(DirectXCommon* dxCommon)override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnLastDraw(DirectXCommon* dxCommon)override;
	void OnFinalize()override;

	void IntroOnUpdate(const float timer)override;
	void IntroMove();
	void ResultOnUpdate(const float timer)override;

	void OnCollision(const std::string& Tag)override;
	//固有関数
	void ShadowUpda();
	void TraceUpda();
	void HitBoundMotion();
	void LimitArea();
	void Move();

	bool PushMoveButton();

	XMFLOAT3 MoveVECTOR(DirectX::XMVECTOR v, float angle);
	XMFLOAT3 GetCameraPos(const float angle, const float str = 10) override;

private:
	std::unique_ptr<FBXObject3d> fbxobj_ = nullptr;
	std::unique_ptr<ParticleEmitter> particleEmitter_ = nullptr;
	std::unique_ptr<Aim> aim_ = nullptr;
	std::unique_ptr<Object2d> shadow_ = nullptr;
	std::list<std::unique_ptr<Trace>> traces_ = {};

	XMFLOAT3 s_rebound_pos_={};
	XMFLOAT3 e_rebound_pos_={};
	XMFLOAT3 aim_pos_ = {};
	XMVECTOR move_ = {};

	//速度関連
	float vel_ = 0;
	bool isFasted = false;
	//ノックバック関連
	bool knockBacking = false;
	const float kKnockBackRange = 15.5f;
	const float kKnockBackFrameMax = 50.0f;
	float knock_back_frame_ = 0.0f;
	float collided_rot_ = 0.0f;

	float y_add = 0.0f;
	int jump_count = 0;
	float shadow_side_ = 0.2f;

	//残像処理
	const int kAftLocateCountMax = 4;
	int  aftImage_count_ = 0;

	//蜂蜜の遅延
	bool onHoney = false;
	const int kHoneyCountMax = 60;
	int honeyCount = 0;
	//足跡パーティクルポップ
	const int kPopTimeMax = 5;
	int particle_pop_time_ = 0;
	int foot_count_ = 0;
	const int kFootCountMax = 10;
	int odd_count_ = 0;
	float foot_rot_ = 0.0f;
};
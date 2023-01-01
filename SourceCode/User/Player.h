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
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinal()override;
	void DebugUpdate()override;
	void IntroOnUpdate(const int& Timer)override;
	void IntroMove();
	void ResultOnUpdate(const int& Timer)override;

	void OnCollision(const std::string& Tag)override;
	void HitBoundMotion();

	void LimitArea();

	void Move();

	XMFLOAT3 MoveVECTOR(DirectX::XMVECTOR v, float angle);
	const DirectX::XMFLOAT3& GetCameraPos(const float& angle, const float& str = 10) override;
	
	void ContactObj();
	XMMATRIX rot2;
	XMFLOAT3 distance{};
	XMFLOAT3 rebound{};
	XMFLOAT3 Start{};
	int holdpos = 0;
	float damageframe = 0.0f;
	bool knockBacking = false;
	Actor* Picmin[30]{};
	float radius = 0;
	bool onHoney = false;
	int honeyCount = 0;
	float speed = 0.0f;
	float vel = 0;
	int charge = 0;
	bool onGround = false;
	//
	// —Ž‰ºƒxƒNƒgƒ‹
	DirectX::XMVECTOR fallV{};

	std::unique_ptr<FBXObject3d> fbxObj;
	ParticleManager* partMan = nullptr;
};
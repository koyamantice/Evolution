#pragma once
#include"Actor.h"
#include"Input.h"
#include<list>
#include"Bullet.h"
#include <sstream>
#include <SourceCode/Obj/2d/Texture.h>
#include<DirectXMath.h>
#include"Aim.h"

class Player :public Actor {
public:
	Player(){};
	void LoadData();
	void UpdateCommand();

	
	const XMFLOAT3& GetLockPos()override { return rockpos;}
private:
	Input* input =Input::GetInstance();

	std::stringstream parameterCommands;
	//std::unique_ptr<Texture> LockOn{};
	std::unique_ptr<Aim> LockOn{};

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

	void Move();

	XMFLOAT3 MoveVECTOR(DirectX::XMVECTOR v, float angle);
	const DirectX::XMFLOAT3& GetCameraPos(float angle) override;
	

	void ContactObj();
	XMMATRIX rot2;

	Actor* Picmin[30]{};
	float radius = 0;


	float vel = 0;
	int charge = 0;
	bool onGround = false;
	//
	// —Ž‰ºƒxƒNƒgƒ‹
	DirectX::XMVECTOR fallV{};

	std::unique_ptr<FBXObject3d> fbxObj;

};
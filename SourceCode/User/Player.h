#pragma once
#include"Actor.h"
#include"Input.h"
#include<list>
#include"Bullet.h"
#include <sstream>
#include <SourceCode/Obj/2d/Texture.h>
#include<DirectXMath.h>

class Player :public Actor {
public:
	Player(){};
	void LoadData();
	void UpdateCommand();

	
	const XMFLOAT3& GetLockPos()override { return rockpos;}
private:
	Input* input =Input::GetInstance();

	std::stringstream parameterCommands;
	std::unique_ptr<Texture> LockOn{};

	XMFLOAT3 rockpos={0.0f,0.5f,0.0f};
	void OnInit()override;
	void OnUpda()override;
	void OnDraw()override;
	void OnFinal()override;
	void DebugUpdate()override;

	void OnCollision(const std::string& Tag)override;

	void Move();

	void RightMove();
	void LeftMove();

	XMFLOAT3 MoveVECTOR(DirectX::XMVECTOR v, float angle);

	void Shot();

	void ContactObj();
	XMMATRIX rot2;

	Actor* Picmin[30]{};
	float angle = 0;
	float radius = 0;
	float vel = 0;
	int charge = 0;
	bool onGround = false;
	//
	// —Ž‰ºƒxƒNƒgƒ‹
	DirectX::XMVECTOR fallV{};


};
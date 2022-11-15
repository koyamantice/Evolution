#pragma once
#include <SourceCode/Obj/2d/Texture.h>
#include<list>
#include <sstream>
#include<DirectXMath.h>
#include"Input.h"
#include"Actor.h"
#include"Bullet.h"
class Player;

class Aim {
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:
	void Init();
	void Upda(float angle);
	void Draw();
	void FirstSet();
private:
	std::unique_ptr<Texture> LockOn{};
	std::unique_ptr<Texture> Whistle{};
	static const int GuidNum = 8;
	std::unique_ptr<Texture> Guid[GuidNum]{};
	std::unique_ptr<Texture> FirstUI{};
	XMFLOAT3 GuidPos[GuidNum];
	Input* input = Input::GetInstance();
	XMFLOAT3 MoveVector(DirectX::XMVECTOR v, float angle);


	bool first = true;

	void Move(float angle);
	void EnemySet();
	float Area = 0.3f;
	float distance = 5.0f;
	float whiframe = 0.0f;
	float alpha= 0.3f;
	bool collect = false;
	Actor* player = nullptr;
	Bullet* bullet = nullptr;
};
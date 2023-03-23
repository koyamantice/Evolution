#pragma once
#include <SourceCode/Obj/2d/Object2d.h>
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
	Input* input = Input::GetInstance();
public:
	void Init();
	void Upda(float angle);
	void Draw();
	void FirstSet();
	void  SetIsActive(const bool& _isActive) { isActive = _isActive; }
private:
	std::unique_ptr<Object2d> LockOn{};
	std::unique_ptr<Object2d> Whistle{};
	static const int GuidNum = 8;
	std::unique_ptr<Object2d> Guid[GuidNum]{};

	enum Explanation {
		SHOT=0,
		RECOVERY,
		RECOVERYPUSH,
		COMMENTMAX
	};

	std::unique_ptr<Object2d> comment_ui_[COMMENTMAX]{};
	std::unique_ptr<AudioManager> audioManager = nullptr;
	std::unique_ptr <ParticleManager> partMan = nullptr;


	XMFLOAT3 GuidPos[GuidNum];
	XMFLOAT3 MoveVector(DirectX::XMVECTOR v, float angle);
	XMFLOAT3 after_pos{};

	int explanation_now_ = 0;
	bool first = true;
	bool second = true;

	int Animation = 0;
	int vel = 1;
	int animeframe = 0;
	float FirstAlpha = 1;
	float SecondAlpha = 1;


	bool enemy_set = false;
	void Move(float angle);
	void EnemySet();
	float Area = 0.3f;
	float distance = 5.0f;
	float whiframe = 0.0f;
	float alpha= 0.3f;
	bool collect = false;
	Actor* player = nullptr;
	Bullet* bullet = nullptr;

	bool isActive = true;
};
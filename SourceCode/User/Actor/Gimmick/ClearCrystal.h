#pragma once
#include"Actor.h"
#include"Input.h"
#include<list>
#include <sstream>
#include <SourceCode/Obj/2d/Object2d.h>
#include"Player.h"


class ClearCrystal :public Actor {
	enum class Phase {
		Approch,
		Leave,
		Wait,
	};
public:
	ClearCrystal() {};
private:
	Input* input = Input::GetInstance();
	std::stringstream parameterCommands;

	void OnInitialize()override;
	void OnUpdate()override;
	void OnDraw(DirectXCommon* dxCommon)override;
	void OnFinalize()override;
	void ResultOnUpdate(const float timer)override;
	void OnCollision(const std::string& Tag)override;
	Actor* player=nullptr;

	std::unique_ptr<ParticleEmitter> partcleEmitter_ = nullptr;

	std::unique_ptr<Object2d> in_out = nullptr;
	
	
	
	
	
	bool ease_ = true;
	float ease_frame = 0.0f;
	const float kEaseFrameMax = 150.0f;



	float frame_ = 0.0f;
	const float kStandFrame = 50.0f;
	float frame_add_ = 1.0f / kStandFrame;

	float stand_radius_ = 0;
	float waitTimer = 0;
	float vel = 0;
};
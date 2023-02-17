#pragma once
#include"Actor.h"

#include"BossLevelLoader.h"
#include "Singleton.h"

#include <SourceCode/Common/Easing.h>

#include<fstream>
#include <sstream>
#include <cstddef>
#include <iostream>

class Boss :public Actor {
public:
	virtual ~Boss() = default;


	void Func1();
	void Func2();
protected:
	void LoadData(const std::string& _bossname);

	void InitCommon(FBXModel* _model=nullptr, XMFLOAT3 _scale = {1,1,1}, XMFLOAT3 _rotation = { 0,0,0 });

	float vel_ = 0;
	float cooltime_ = 0;


	static void (Boss::*phaseFuncTable[])();
	int phase_ = 0;


	int animation_count_ = 0;
	const float kPredictTime = 240;
	const float kScaleCount = 4.0f;
	const float kAttackTime = 150;
	float vel = 0;
	float scale = 0.01f;
	float scaframe = 0;






	Actor* player_ = nullptr;
	BossLevelLoader::LevelData levelData_ = {};

	std::unique_ptr<Object2d> shadow_ = nullptr;
	std::unique_ptr<FBXObject3d> fbxObject_;
	std::unique_ptr <ParticleEmitter> particle_ = nullptr;

};


#pragma once
#include <list>
#include <memory>
#include<cassert>
#include <vector>
#include <map>


#include <SourceCode/System/DirectXCommon.h>
#include <LevelLoader.h>

#include"FBXObject3d.h"
#include"Object3d.h"

#include <ParticleManager.h>


class TitleText {
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
public:
	TitleText();
	~TitleText();

	void Init();
	void Upda();
	void Draw(DirectXCommon* dxCommon);

private:

	void DoorUpdate();

private:
	std::unique_ptr<Object3d> texts[6];
	std::unique_ptr<Object3d> door;
	std::list<std::unique_ptr<Object3d>> grounds;
	std::map<std::string, Model*> models;

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelFighter = nullptr;
	Model* modelSphere = nullptr;
	Model* modelPine = nullptr;


	LevelData* levelData = nullptr;


	enum DoorStatus {
		kOpening = 1,
		kClosing = -1
	};
	float frame = 0;
	float door_vel = 0.01f;
	int door_status = kOpening;




	ParticleManager* partMan = nullptr;


	XMFLOAT3 pos[6] = {
	{24.5f,5,-15.5f},
	{22.5f,5,-10.5f},
	{19.5f,5,-4.5f},
	{19.5f,5, 4.5f},
	{22.5f,5,9.5f},
	{24.5f,5,15.5f},

	};

};

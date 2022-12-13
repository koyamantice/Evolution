#pragma once
#include"Object3d.h"
#include"FBXObject3d.h"
#include <list>
#include <memory>
#include<cassert>
#include <SourceCode/System/DirectXCommon.h>
#include <LevelLoader.h>
#include <vector>
#include <map>


class TitleText {
public:
	TitleText();
	~TitleText();

	void Init();
	void Upda();
	void Draw(DirectXCommon* dxCommon);


private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
private:
	std::unique_ptr<Object3d> texts[6];
	std::list<std::unique_ptr<Object3d>> grounds;
	std::map<std::string, Model*> models;

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelFighter = nullptr;
	Model* modelSphere = nullptr;
	Model* modelPine = nullptr;


	LevelData* levelData = nullptr;


	float pos[6]{};

	float frame=0;
};

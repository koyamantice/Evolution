#pragma once
#include <string>
#include "Model.h"
#include <map>
#include <FBXModel.h>

class ModelManager final {
private:
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager& r) = default;
	ModelManager& operator= (const ModelManager& r) = default;

public:

	enum ModelName {
		Player,
		EHub,
		Enemy,
		Fork,
		Water,
		Kyusu,
		LeftShoes,
		RightShoes,
		Pastel,
		Platform,
		skydome,
		Piyopiyo,
		Particle,
		Mill,
		SiroMotti,
		LeftHand_Open,
		LeftHand_Close,
		RightHand_Open,
		RightHand_Close,
		Motti_Bullet
	};
	enum FBXName {
		MottiMove,
		Motti_No_Move
	};

public:
	static ModelManager* GetIns();		//�擾�p

	void Initialize();
	void LoadModel(const ModelName modelName, std::string fileName);
	void LoadFBXModel(const FBXName modelName, std::string fileName);
	Model* GetModel(ModelName modelName) { return model[modelName]; }
	FBXModel* GetFBXModel(FBXName FBXName) { return fbxModel[FBXName]; }
private:
	static std::map<ModelName, Model*> model;	//���f���i�[�}�b�v
	static std::map<FBXName, FBXModel*> fbxModel;
};
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
		kSkydome = 0,
		kGround,
		kGoalcrystal,
		kTextG,
		kTextN,
		kTextO,
		kTextM,
		kTextE,
		kHouse,
		kHoney,
		kTouch,
		kDoor,
	};
	enum FBXName {
		kMash = 0,
		kSeed,
		kRedkobito,
		kGreenkobito,
		kBee,
	};

public:
	static ModelManager* GetIns();		//取得用

	void Initialize();
	void Finalize();
	void LoadModel(const ModelName modelName, std::string fileName);
	void LoadFBXModel(const FBXName modelName, std::string fileName);
	Model* GetModel(ModelName modelName) { return model[modelName]; }
	FBXModel* GetFBXModel(FBXName FBXName) { return fbxModel[FBXName]; }
private:
	static std::map<ModelName, Model*> model;	//モデル格納マップ
	static std::map<FBXName, FBXModel*> fbxModel;
};
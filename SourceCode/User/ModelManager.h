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
		kDungeon,
		kSmallMash,
		kTextG,
		kTextN,
		kTextO,
		kTextM,
		kTextE,
		kHouse,
		kNest,
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
		kCow,
	};

public:
	static ModelManager* GetInstance();		//�擾�p

	void Initialize();
	void Finalize();
	void LoadModel(const ModelName modelName, std::string fileName);
	void LoadFBXModel(const FBXName modelName, std::string fileName);
	Model* GetModel(ModelName modelName) { return model_[modelName]; }
	FBXModel* GetFBXModel(FBXName FBXName) { return fbxModel_[FBXName]; }
private:
	static std::map<ModelName, Model*> model_;	//���f���i�[�}�b�v
	static std::map<FBXName, FBXModel*> fbxModel_;
};
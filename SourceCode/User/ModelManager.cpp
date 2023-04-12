#include"ModelManager.h"
#include "FbxLoader.h"

std::map<ModelManager::ModelName, Model*> ModelManager::model_;
std::map<ModelManager::FBXName, FBXModel*>  ModelManager::fbxModel_;

ModelManager* ModelManager::GetInstance() {
	static ModelManager instans;
	return &instans;
}

void ModelManager::Initialize() {
	//OBJフォルダを読み込みます。
	LoadModel(kSkydome, "Skydome");
	LoadModel(kDungeon, "DemoDungeon");
	LoadModel(kSmallMash, "SmallMash");
	LoadModel(kTextG, "text01G");
	LoadModel(kTextN, "text02N");
	LoadModel(kTextO, "text03O");
	LoadModel(kTextM, "text04M");
	LoadModel(kTextE, "text05E");
	LoadModel(kNest, "nest");
	LoadModel(kHoney, "honey");
	LoadModel(kTouch, "TouchNo");
	LoadModel(kDoor, "Door");

	//FBXフォルダを読み込みます。
	LoadFBXModel(kMash,"Mash");
	LoadFBXModel(kSeed,"Player");
	LoadFBXModel(kRedkobito, "RedKobito");
	LoadFBXModel(kGreenkobito, "kobi");
	LoadFBXModel(kBee, "Bee");
	LoadFBXModel(kCow, "Player");

}

void ModelManager::Finalize() {
	model_.clear();
	fbxModel_.clear();
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName) {
	model_[modelName] = new Model();
	model_[modelName] = Model::CreateFromOBJ(fileName);
}
void ModelManager::LoadFBXModel(const FBXName modelName, std::string fileName) {
	fbxModel_[modelName] = new FBXModel();
	fbxModel_[modelName] = FbxLoader::GetInstance()->LoadModelFromFile(fileName);
}
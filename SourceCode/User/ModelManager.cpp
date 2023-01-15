#include"ModelManager.h"
#include "FbxLoader.h"

std::map<ModelManager::ModelName, Model*> ModelManager::model;
std::map<ModelManager::FBXName, FBXModel*>  ModelManager::fbxModel;

ModelManager* ModelManager::GetIns() {
	static ModelManager instans;
	return &instans;
}

void ModelManager::Initialize() {
	//OBJフォルダを読み込みます。
	LoadModel(kSkydome, "Skydome");
	LoadModel(kGround, "DemoDungeon");
	LoadModel(kGoalcrystal, "MonolithCrystal");
	LoadModel(kTextG, "text01G");
	LoadModel(kTextN, "text02N");
	LoadModel(kTextO, "text03O");
	LoadModel(kTextM, "text04M");
	LoadModel(kTextE, "text05E");
	LoadModel(kHouse, "House");
	LoadModel(kHoney, "Honey");
	LoadModel(kTouch, "TouchNo");
	LoadModel(kDoor, "Door");

	//FBXフォルダを読み込みます。
	LoadFBXModel(kMash,"Mash");
	LoadFBXModel(kSeed,"Player");
	LoadFBXModel(kRedkobito, "RedKobito");
	LoadFBXModel(kGreenkobito, "kobi");
	LoadFBXModel(kBee, "Bee");

}

void ModelManager::Finalize() {
	model.clear();
	fbxModel.clear();
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName) {
	model[modelName] = new Model();
	model[modelName] = Model::CreateFromOBJ(fileName);
}
void ModelManager::LoadFBXModel(const FBXName modelName, std::string fileName) {
	fbxModel[modelName] = new FBXModel();
	fbxModel[modelName] = FbxLoader::GetInstance()->LoadModelFromFile(fileName);
}
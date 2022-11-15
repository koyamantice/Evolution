#include"ModelManager.h"
#include "FbxLoader.h"

std::map<ModelManager::ModelName, Model*> ModelManager::model;
std::map<ModelManager::FBXName, FBXModel*>  ModelManager::fbxModel;

ModelManager* ModelManager::GetIns() {
	static ModelManager instans;
	return &instans;
}

void ModelManager::Initialize() {
	LoadModel(Player, "chr_knight");
	LoadModel(Enemy, "ball");
	LoadModel(skydome, "skydome");
	LoadModel(Ground, "Dungeon");
	LoadModel(hole, "hole");
	LoadModel(Crystal, "Crystal");
	LoadModel(Goal, "MonolithCrystal");
	LoadModel(textG, "text01G");
	LoadModel(textN, "text02N");
	LoadModel(textO, "text03O");
	LoadModel(textM, "text04M");
	LoadModel(textE, "text05E");
	LoadModel(RedKobi,"RedKobito");
	
	LoadFBXModel(Mash,"Mash");
	LoadFBXModel(Bird,"Player");
	LoadFBXModel(Kobi, "kobi");
	LoadFBXModel(brave, "Eagle");
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
#include "SceneManager.h"
#include<cassert>
void SceneManager::Finalize() {
	//最後のシーンの終了と開放
	scene_->Finalize();
	scene_.reset();
}

SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;
	return &instance;
}

void SceneManager::Update(DirectXCommon* dxCommon) {
	//シーン切り替え
	if (nextScene_) {
		//旧シーンの終了
		if (scene_) {
			scene_->Finalize();
			scene_.reset();
		}
		scene_= std::move(nextScene_);
		nextScene_.reset();
		scene_->Initialize(dxCommon);
	}
	scene_->Update(dxCommon);
}

void SceneManager::Draw(DirectXCommon* dxCommon) {
	scene_->Draw(dxCommon);
}

void SceneManager::ChangeScene(const std::string& sceneName) {
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);
	//次のシーン生成
	nextScene_= sceneFactory_->CreateScene(sceneName);
}

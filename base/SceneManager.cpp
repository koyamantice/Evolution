#include "SceneManager.h"
#include<cassert>
void SceneManager::Finalize() {
	//�Ō�̃V�[���̏I���ƊJ��
	scene_->Finalize();
	delete scene_;
}

SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;
	return &instance;
}

void SceneManager::Update(DirectXCommon* dxCommon) {
	//�V�[���؂�ւ�
	if (nextScene_) {
		//���V�[���̏I��
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}
		scene_ = nextScene_;
		nextScene_ = nullptr;
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
	//���̃V�[������
	nextScene_=sceneFactory_->CreateScene(sceneName);
}

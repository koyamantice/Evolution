#include "SceneFactory.h"
#include "TitleScene.h"


BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//���̃V�[���̍쐬
	BaseScene* newScene = nullptr;
	if (sceneName=="TITLE") {
		newScene=new TitleScene();
	}
	return newScene;
}

#include "SceneFactory.h"
#include "TitleScene.h"


BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//次のシーンの作成
	BaseScene* newScene = nullptr;
	if (sceneName=="TITLE") {
		newScene=new TitleScene();
	}
	return newScene;
}

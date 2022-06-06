#include "SceneFactory.h"
#include "TitleScene.h"


BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//Ÿ‚ÌƒV[ƒ“‚Ìì¬
	BaseScene* newScene = nullptr;
	if (sceneName=="TITLE") {
		newScene=new TitleScene();
	}
	return newScene;
}

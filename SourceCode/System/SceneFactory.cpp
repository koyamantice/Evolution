#include "SceneFactory.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "DebugScene.h"
#include "MapScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//éüÇÃÉVÅ[ÉìÇÃçÏê¨
	BaseScene* newScene = nullptr;
	if (sceneName=="TITLE") {
		newScene=new TitleScene();
	}
	if (sceneName == "PLAY") {
		newScene = new PlayScene();
	}
	if (sceneName == "DEBUG") {
		newScene = new DebugScene();
	}
	if (sceneName=="MAP") {
		newScene = new MapScene();
	}
	return newScene;
}

#include "SceneFactory.h"
#include "TitleScene.h"
#include "FirstStage.h"
#include "DebugScene.h"
#include "SecondStage.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//éüÇÃÉVÅ[ÉìÇÃçÏê¨
	BaseScene* newScene = nullptr;
	if (sceneName=="TITLE") {
		newScene=new TitleScene();
	}
	if (sceneName == "PLAY") {
		newScene = new FirstStage();
	}
	if (sceneName == "DEBUG") {
		newScene = new DebugScene();
	}
	if (sceneName=="MAP") {
		newScene = new SecondStage();
	}
	return newScene;
}

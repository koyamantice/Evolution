#include "SceneFactory.h"
#include "TitleScene.h"
#include "FirstStage.h"
#include "SecondStage.h"
#include "MSecondStage.h"
#include "ThirdStage.h"
#include "MThirdStage.h"


BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//éüÇÃÉVÅ[ÉìÇÃçÏê¨
	BaseScene* newScene = nullptr;
	if (sceneName=="TITLE") {
		newScene=new TitleScene();
	}
	if (sceneName == "FIRSTSTAGE") {
		newScene = new FirstStage();
	}
	if (sceneName == "MSECOND") {
		newScene = new MSecondStage();
	}
	if (sceneName=="SECONDSTAGE") {
		newScene = new SecondStage();
	}
	if (sceneName == "MTHIRD") {
		newScene = new MThirdStage();
	}
	if (sceneName == "THIRDSTAGE") {
		newScene = new ThirdStage();
	}

	return newScene;
}

#include "SceneFactory.h"
#include "TitleScene.h"
#include "FirstStage.h"
#include "SecondStage.h"
#include "MSecondStage.h"
#include "ThirdStage.h"
#include "MThirdStage.h"


std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName) {
	//éüÇÃÉVÅ[ÉìÇÃçÏê¨
	std::unique_ptr<BaseScene> newScene = nullptr;
	if (sceneName == "TITLE") {
		newScene = std::make_unique<TitleScene>();
	}
	if (sceneName == "FIRSTSTAGE") {
		newScene = std::make_unique < FirstStage>();
	}
	if (sceneName == "MSECOND") {
		newScene = std::make_unique < MSecondStage>();
	}
	if (sceneName == "SECONDSTAGE") {
		newScene = std::make_unique < SecondStage>();
	}
	if (sceneName == "MTHIRD") {
		newScene = std::make_unique < MThirdStage>();
	}
	if (sceneName == "THIRDSTAGE") {
		newScene = std::make_unique < ThirdStage>();
	}

	return newScene;
}

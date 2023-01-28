#include "SceneFactory.h"
#include "TitleScene.h"
#include "FirstStage.h"
#include "SecondStage.h"
#include "MSecondStage.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//���̃V�[���̍쐬
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
	return newScene;
}

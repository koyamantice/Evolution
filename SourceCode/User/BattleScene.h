#pragma once
#include "SceneManager.h"




class BattleScene : public BaseScene {

public:
	virtual ~BattleScene() = default;

protected:
	void BattleInit();



protected:

	std::unique_ptr<PauseUI> pauseUi{};

	std::unique_ptr<Sprite>	Screen[2]{};

	std::unique_ptr<Sprite> FeedBlack{};
	float feedAlpha = 0.0f;


	//killƒNƒŠƒbƒv
	int finishTime = 0;

	bool Intro = true;
	bool Result = false;
	bool GameOver = false;


};
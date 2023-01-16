#pragma once
#include "SceneManager.h"




class BattleScene : public BaseScene {


protected:

	std::unique_ptr<PauseUI> pauseUi{};

	std::unique_ptr<Sprite>	Screen[2]{};




};
#include "BattleScene.h"

void BattleScene::BattleInit() {
	//導入枠
	Sprite* _Screen = nullptr;
	_Screen = Sprite::Create(ImageManager::SceneCover, { 0,0 });
	Screen[0].reset(_Screen);
	Sprite* _Screen2 = nullptr;
	_Screen2 = Sprite::Create(ImageManager::SceneCover, { 0,600 });
	Screen[1].reset(_Screen2);

	//
	PauseUI* pause_ui = new PauseUI();
	pauseUi.reset(pause_ui);

	//スプライト生成
	Sprite* Effect_ = Sprite::Create(ImageManager::Black, { 0.0f,0.0f });
	FeedBlack.reset(Effect_);
	FeedBlack->SetColor({ 1,1,1,feedAlpha });

}

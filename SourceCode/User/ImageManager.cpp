#include "ImageManager.h"

ImageManager* ImageManager::GetIns() {
	static ImageManager instans;
	return &instans;
}

void ImageManager::Load2D() {
	// テクスチャ読み込み
	Sprite::LoadTexture(Test, L"Resources/2d/Debug/Test.png");
	Sprite::LoadTexture(DebugBack, L"Resources/2d/Debug/Back.png");
	Sprite::LoadTexture(Black, L"Resources/2d/Effect/Blackfilter.png");
	Sprite::LoadTexture(Title,L"Resources/2d/Title/TitleBack.png");
	Sprite::LoadTexture(Clear, L"Resources/2d/PlayUI/GameClear.png");
	Sprite::LoadTexture(Over, L"Resources/2d/PlayUI/GameOver.png");
	Sprite::LoadTexture(UISheet, L"Resources/2d/PauseUi/UISheet.png");
	Sprite::LoadTexture(TitleBack, L"Resources/2d/PauseUi/TitleBack.png");
	Sprite::LoadTexture(Button, L"Resources/2d//Title/decide.png");
	Sprite::LoadTexture(DebugMove, L"Resources/2d/Title/Debug.png");
	Sprite::LoadTexture(TitleMove, L"Resources/2d/Title/Title.png");
	Sprite::LoadTexture(Bar, L"Resources/2d/PauseUi/Bar.png");
	Sprite::LoadTexture(ZBack, L"Resources/2d/PauseUi/ZBack.png");
	Sprite::LoadTexture(Option, L"Resources/2d/PauseUi/Option.png");
	Sprite::LoadTexture(Vignette, L"Resources/2d/PlayUI/Vignette.png");
	Sprite::LoadTexture(PlayerCover, L"Resources/2d/PlayUI/PlayerCover.png");
	Sprite::LoadTexture(CharaCover, L"Resources/2d/PlayUI/CharaCover2.png");
	Sprite::LoadTexture(Num, L"Resources/2d/PlayUI/Num.png"); 
	Sprite::LoadTexture(numBack, L"Resources/2d/PlayUI/numBack.png");
	Sprite::LoadTexture(slash, L"Resources/2d/PlayUI/slash.png");
	Sprite::LoadTexture(SceneCover, L"Resources/2d/Effect/SceneCover.png");
	Sprite::LoadTexture(Intro01, L"Resources/2d/IntroWord/Intro01.png");
	Sprite::LoadTexture(Intro02, L"Resources/2d/IntroWord/Intro02.png");
	Sprite::LoadTexture(Intro03, L"Resources/2d/IntroWord/Intro03.png");
	Sprite::LoadTexture(Intro04, L"Resources/2d/IntroWord/Intro04.png");
	Sprite::LoadTexture(Intro05, L"Resources/2d/IntroWord/Intro05.png");
	Sprite::LoadTexture(Intro06, L"Resources/2d/IntroWord/Intro06.png");
	Sprite::LoadTexture(CameCon, L"Resources/2d/PlayUI/CameCon.png");
	Sprite::LoadTexture(Rockon, L"Resources/2d/PlayUI/Rockon.png");
	Sprite::LoadTexture(HpCover , L"Resources/2d/PlayUI/HpCover2.png");
	Sprite::LoadTexture(HpGreen, L"Resources/2d/PlayUI/Hp_Green.png");
	Sprite::LoadTexture(HpRed, L"Resources/2d/PlayUI/Hp_Red.png");
	Sprite::LoadTexture(Damage, L"Resources/2d/PlayUI/Damage.png");



}

void ImageManager::LoadTex2D() {
	// テクスチャ読み込み
	Object2d::LoadTexture(Lock, L"Resources/2d/PlayUI/Lock.png");
	Object2d::LoadTexture(Battle, L"Resources/2d/PlayUI/Battle.png");
	Object2d::LoadTexture(Fire, L"Resources/2d/Effect/Fire.png");
	Object2d::LoadTexture(InOut, L"Resources/2d/Effect/Base.png");
	Object2d::LoadTexture(Guid, L"Resources/2d/Effect/Diamond.png");
	Object2d::LoadTexture(Shadow, L"Resources/2d/Effect/Shadow.png");
	Object2d::LoadTexture(RedDead, L"Resources/2d/Effect/CharaDead.png");
	Object2d::LoadTexture(GreenDead, L"Resources/2d/Effect/CharaDead.png");
	Object2d::LoadTexture(SlowUI, L"Resources/2d/PlayUI/SlowUI.png");
	Object2d::LoadTexture(SetUI, L"Resources/2d/PlayUI/SetUI.png");
	Object2d::LoadTexture(SetUI2, L"Resources/2d/PlayUI/SetUI2.png");


}


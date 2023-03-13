#include "ImageManager.h"
#include <ParticleManager.h>


bool ImageManager::load = false;

ImageManager* ImageManager::GetIns() {
	static ImageManager instans;
	return &instans;
}

void ImageManager::Load2D() {
	// テクスチャ読み込み(リソース2dのフォルダーからしか受け取らないようにする)
	Sprite::LoadTexture(Test, L"Resources/2d/Debug/Test.png");
	Sprite::LoadTexture(Black, L"Resources/2d/Effect/Blackfilter.png");
	Sprite::LoadTextureDDS(Title,L"Resources/2d/Title/TitleBack.dds");
	Sprite::LoadTexture(DebugMove, L"Resources/2d/Title/Debug.png");
	Sprite::LoadTexture(noDebugMove, L"Resources/2d/Title/Debug2.png");
	Sprite::LoadTexture(TitleMove, L"Resources/2d/Title/Title.png");
	Sprite::LoadTexture(noTitleMove, L"Resources/2d/Title/Title2.png");

	Sprite::LoadTexture(Clear, L"Resources/2d/PlayUI/GameClear.png");
	Sprite::LoadTexture(Over, L"Resources/2d/PlayUI/gameover.png");

	Sprite::LoadTexture(UISheet, L"Resources/2d/PauseUi/UISheet.png");
	Sprite::LoadTexture(optionline, L"Resources/2d/PauseUi/optionline1.png");
	Sprite::LoadTexture(TitleBack, L"Resources/2d/PauseUi/TitleBack.png");
	Sprite::LoadTexture(Bar, L"Resources/2d/PauseUi/Bar.png");
	Sprite::LoadTexture(ZBack, L"Resources/2d/PauseUi/ZBack.png");

	Sprite::LoadTexture(Option, L"Resources/2d/PauseUi/Option.png");
	Sprite::LoadTexture(CameraOpt, L"Resources/2d/PauseUi/optioncamera.png");
	Sprite::LoadTexture(Normal, L"Resources/2d/PauseUi/normal.png");
	Sprite::LoadTexture(Reverse, L"Resources/2d/PauseUi/reverse.png");

	Sprite::LoadTexture(CameraBar, L"Resources/2d/PauseUi/cameraBar.png");
	Sprite::LoadTexture(OptionBack, L"Resources/2d/PauseUi/Back.png");

	Sprite::LoadTexture(kOptionConfig, L"Resources/2d/PauseUi/optionConfig.png");
	Sprite::LoadTexture(kTitleOk, L"Resources/2d/PauseUi/TitleOk.png");
	Sprite::LoadTexture(kTitleCancel, L"Resources/2d/PauseUi/TitleNo.png");
	Sprite::LoadTexture(ConfigBar, L"Resources/2d/PauseUi/cameraBar.png");

	Sprite::LoadTexture(CharaCover, L"Resources/2d/PlayUI/CharaCover2.png");
	Sprite::LoadTexture(Num, L"Resources/2d/PlayUI/Num.png"); 
	Sprite::LoadTexture(numBack, L"Resources/2d/PlayUI/numBack.png");
	Sprite::LoadTexture(slash, L"Resources/2d/PlayUI/slash.png");
	Sprite::LoadTexture(SceneCover, L"Resources/2d/Effect/SceneCover.png");
	Sprite::LoadTexture(Skip, L"Resources/2d/Effect/skip.png");

	Sprite::LoadTexture(Intro01, L"Resources/2d/IntroWord/Intro01.png");
	Sprite::LoadTexture(Intro02, L"Resources/2d/IntroWord/Intro02.png");
	Sprite::LoadTexture(Intro03, L"Resources/2d/IntroWord/Intro03.png");
	Sprite::LoadTexture(Intro04, L"Resources/2d/IntroWord/Intro04.png");
	Sprite::LoadTexture(Intro05, L"Resources/2d/IntroWord/Intro05.png");
	Sprite::LoadTexture(Intro06, L"Resources/2d/IntroWord/Intro06.png");

	Sprite::LoadTexture(kStartOption, L"Resources/2d/PlayUI/Option.png");
	Sprite::LoadTexture(CameCon, L"Resources/2d/PlayUI/CameCon.png");
	Sprite::LoadTexture(Con_vis, L"Resources/2d/PlayUI/control.png");
	Sprite::LoadTexture(ControlRT, L"Resources/2d/PlayUI/controlRT.png");
	Sprite::LoadTexture(ControlY, L"Resources/2d/PlayUI/controlY.png");

	Sprite::LoadTexture(HpCover , L"Resources/2d/PlayUI/HpCover2.png");
	Sprite::LoadTextureDDS(HpGreen, L"Resources/2d/PlayUI/Hp_Green.dds");
	Sprite::LoadTextureDDS(HpRed, L"Resources/2d/PlayUI/Hp_Red.dds");

	Sprite::LoadTexture(Damage, L"Resources/2d/PlayUI/Damage.png");

	Sprite::LoadTexture(kMissionMsecond, L"Resources/2d/IntroWord/MSecondMission.png");
	Sprite::LoadTexture(kMsecondNum, L"Resources/2d/IntroWord/MSecondMissionNum.png");

	Sprite::LoadTexture(kMsecondNum, L"Resources/2d/IntroWord/MSecondMissionNum.png");


}

void ImageManager::LoadTex2D() {
	// テクスチャ読み込み
	Object2d::LoadTexture(Lock, L"Resources/2d/PlayUI/Lock.png");
	Object2d::LoadTexture(Battle, L"Resources/2d/PlayUI/Battle.png");
	Object2d::LoadTexture(Fire, L"Resources/2d/Effect/Fire.png");
	Object2d::LoadTexture(InOut, L"Resources/2d/Effect/Base.png");
	Object2d::LoadTexture(Guid, L"Resources/2d/Effect/Diamond.png");
	Object2d::LoadTexture(Shadow, L"Resources/2d/Effect/Shadow.png");
	Object2d::LoadTexture(RedDead, L"Resources/2d/Effect/DeadRed.png");
	Object2d::LoadTexture(GreenDead, L"Resources/2d/Effect/CharaDead.png");
	Object2d::LoadTexture(SlowUI, L"Resources/2d/PlayUI/SlowUI.png");
	Object2d::LoadTexture(SetUI, L"Resources/2d/PlayUI/SetUI.png");
	Object2d::LoadTexture(SetUI2, L"Resources/2d/PlayUI/SetUI2.png");
	Object2d::LoadTexture(kMission_0,L"Resources/2d/PlayUI/Number/mission_0.png");
	Object2d::LoadTexture(kMission_1, L"Resources/2d/PlayUI/Number/mission_1.png");
	Object2d::LoadTexture(kMission_2, L"Resources/2d/PlayUI/Number/mission_2.png");
	Object2d::LoadTexture(kMission_3, L"Resources/2d/PlayUI/Number/mission_3.png");
	Object2d::LoadTexture(kMission_4, L"Resources/2d/PlayUI/Number/mission_4.png");
	Object2d::LoadTexture(kMission_5, L"Resources/2d/PlayUI/Number/mission_5.png");
	Object2d::LoadTexture(kMission_s, L"Resources/2d/PlayUI/Number/mission_slash.png");
	Object2d::LoadTexture(kpredicted, L"Resources/2d//Effect/predicted.png");

}

void ImageManager::LoadParticle() {

	ParticleManager::LoadTexture(nul, "shine");
	ParticleManager::LoadTexture(smoke, "Smoke");
	ParticleManager::LoadTexture(fire, "smoke1");
	ParticleManager::LoadTexture(charge, "Charge");

	load = true;
}


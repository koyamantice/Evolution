#pragma once
#include <map>
#include <string>
#include "Sprite.h"
#include "Object2d.h"
class ImageManager  final {
private:
	ImageManager() = default;
	~ImageManager() = default;
	ImageManager(const ImageManager & r) = default;
	ImageManager& operator= (const ImageManager & r) = default;

public:
	enum texName {
		Test,
		DebugBack,
		Black,
		Title,
		Clear,
		Over,
		UISheet,
		optionline,
		TitleBack,
		Button,
		DebugMove,
		TitleMove,
		Bar,
		ZBack,
		Option,
		Vignette,
		PlayerCover,
		CharaCover,
		Num,
		numBack,
		slash,
		SceneCover,
		Intro01,
		Intro02,
		Intro03,
		Intro04,
		Intro05,
		Intro06,
		CameCon,
		Rockon,
		HpCover,
		HpGreen,
		HpRed,
		Damage,
	};
	enum tex2dName {
		Lock,
		Battle,
		Fire,
		InOut,
		Guid,
		Shadow,
		RedDead,
		GreenDead,
		SlowUI,
		SetUI,
		SetUI2,
		kMission_0,
		kMission_1,
		kMission_2,
		kMission_3,
		kMission_4,
		kMission_5,
		kMission_s
	};
	enum particleName {
		nul = 0,
		smoke,
		fire,
		charge,
	
	};
public:
	static ImageManager* GetIns();		//Žæ“¾—p

	void Load2D();
	void LoadTex2D();
	void LoadParticle();

	void Finalize();
};


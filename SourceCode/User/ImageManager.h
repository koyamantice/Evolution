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
		Black,
		Title,
		DebugMove,
		noDebugMove,
		TitleMove,
		noTitleMove,

		Clear,
		Over,

		UISheet,
		optionline,
		TitleBack,
		ZBack,
		Option,
		Bar,


		CameraOpt,
		Normal,
		Reverse,

		CameraBar,
		OptionBack,

		kOptionConfig,
		kTitleOk,

		CharaCover,

		Num,
		numBack,
		slash,
		SceneCover,
		Skip,

		Intro01,
		Intro02,
		Intro03,
		Intro04,
		Intro05,
		Intro06,
		kStartOption,
		CameCon,
		Con_vis,
		ControlRT,
		ControlY,

		HpCover,
		HpGreen,
		HpRed,
		Damage,
		kMissionMsecond,
		kMsecondNum,
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
		kMission_s,
		kpredicted,
	};
	enum particleName {
		nul = 0,
		smoke,
		fire,
		charge,
	
	};
public:
	static ImageManager* GetIns();		//Žæ“¾—p

	static bool load;
	void Load2D();
	void LoadTex2D();
	void LoadParticle();

	void Finalize();
};


#pragma once
#include <map>
#include <string>
#include "Sprite.h"
#include "Texture.h"
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
	};
	enum tex2dName {
		Lock,
		Battle,
		Fire,
		InOut,
		Guid,
		Shadow,
		RedDead,
	};
public:
	static ImageManager* GetIns();		//Žæ“¾—p

	void Load2D();
	void LoadTex2D();
	void Finalize();
};


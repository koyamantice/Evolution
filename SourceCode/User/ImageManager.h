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
	};
	enum tex2dName {
		Lock,
	};
public:
	static ImageManager* GetIns();		//Žæ“¾—p

	void Load2D();
	void LoadTex2D();

};


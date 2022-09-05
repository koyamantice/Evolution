#pragma once
#include "Easing.h"
#include "Sprite.h"
#include<vector>
#include <memory>
#include<string>

class SceneChanger
{
public:
	SceneChanger();
	~SceneChanger();

	static void Init();
	static void Upda();
	static void Draw();

private:
	std::unique_ptr<Sprite>  ImageChanger;
	int Category;
	enum Category{



	};

};
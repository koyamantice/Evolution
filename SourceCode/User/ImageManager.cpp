#include "ImageManager.h"

ImageManager* ImageManager::GetIns() {
	static ImageManager instans;
	return &instans;
}

void ImageManager::Load2D() {
	// テクスチャ読み込み
	Sprite::LoadTexture(Test, L"Resources/2d/Test.png");
	Sprite::LoadTexture(Title,L"Resources/2d/Title.png");
	Sprite::LoadTexture(Clear, L"Resources/2d/GameClear.png");
	Sprite::LoadTexture(DebugBack, L"Resources/2d/Back.png");
	Sprite::LoadTexture(UISheet, L"Resources/2d/PauseUi/UISheet.png");
	Sprite::LoadTexture(TitleBack, L"Resources/2d/PauseUi/TitleBack.png");
	Sprite::LoadTexture(ZBack, L"Resources/2d/PauseUi/ZBack.png");
	Sprite::LoadTexture(Option, L"Resources/2d/PauseUi/Option.png");


}

void ImageManager::LoadTex2D() {
	// テクスチャ読み込み
	Texture::LoadTexture(Lock, L"Resources/2d/Lock.png");
	Texture::LoadTexture(Battle, L"Resources/2d/Battle.png");

}


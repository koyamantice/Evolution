#include "ImageManager.h"

ImageManager* ImageManager::GetIns() {
	static ImageManager instans;
	return &instans;
}

void ImageManager::Load2D() {
	// テクスチャ読み込み
	Sprite::LoadTexture(Test, L"Resources/2d/Test.png");
	Sprite::LoadTexture(DebugBack, L"Resources/2d/Back.png");
	Sprite::LoadTexture(Black, L"Resources/2d/Effect/Blackfilter.png");
	Sprite::LoadTexture(Title,L"Resources/2d/Title.png");
	Sprite::LoadTexture(Clear, L"Resources/2d/GameClear.png");
	Sprite::LoadTexture(UISheet, L"Resources/2d/PauseUi/UISheet.png");
	Sprite::LoadTexture(TitleBack, L"Resources/2d/PauseUi/TitleBack.png");
	Sprite::LoadTexture(ZBack, L"Resources/2d/PauseUi/ZBack.png");
	Sprite::LoadTexture(Option, L"Resources/2d/PauseUi/Option.png");
	Sprite::LoadTexture(Vignette, L"Resources/2d/Vignette.png");
	Sprite::LoadTexture(PlayerCover, L"Resources/2d/PlayerCover.png");
	Sprite::LoadTexture(CharaCover, L"Resources/2d/CharaCover2.png");
	Sprite::LoadTexture(Num, L"Resources/2d/Num.png"); 
	Sprite::LoadTexture(numBack, L"Resources/2d/numBack.png");
	Sprite::LoadTexture(slash, L"Resources/2d/slash.png");
}

void ImageManager::LoadTex2D() {
	// テクスチャ読み込み
	Texture::LoadTexture(Lock, L"Resources/2d/Lock.png");
	Texture::LoadTexture(Battle, L"Resources/2d/Battle.png");
	Texture::LoadTexture(Fire, L"Resources/2d/Effect/Fire.png");
	Texture::LoadTexture(InOut, L"Resources/2d/Effect/Base.png");
	Texture::LoadTexture(Guid, L"Resources/2d/Effect/Diamond.png");
	Texture::LoadTexture(Shadow, L"Resources/2d/Effect/Shadow.png");

}


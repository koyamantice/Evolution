#include "ImageManager.h"

ImageManager* ImageManager::GetIns() {
	static ImageManager instans;
	return &instans;
}

void ImageManager::Load2D() {
	// テクスチャ読み込み
	Sprite::LoadTexture(Test, L"Resources/2d/Test.png");
}

void ImageManager::LoadTex2D() {
	// テクスチャ読み込み
//Texture::LoadTexture(0, L"Resources/2d/enemy.png");
}

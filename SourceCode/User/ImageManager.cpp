#include "ImageManager.h"

ImageManager* ImageManager::GetIns() {
	static ImageManager instans;
	return &instans;
}

void ImageManager::Load2D() {
	// テクスチャ読み込み
	//Sprite::LoadTexture(0, L"Resources/2d/PlayerHP.png");
}

void ImageManager::LoadTex2D() {
	// テクスチャ読み込み
//Texture::LoadTexture(0, L"Resources/2d/enemy.png");
}

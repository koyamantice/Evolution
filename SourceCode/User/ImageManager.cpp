#include "ImageManager.h"

ImageManager* ImageManager::GetIns() {
	static ImageManager instans;
	return &instans;
}

void ImageManager::Load2D() {
	// �e�N�X�`���ǂݍ���
	//Sprite::LoadTexture(0, L"Resources/2d/PlayerHP.png");
}

void ImageManager::LoadTex2D() {
	// �e�N�X�`���ǂݍ���
//Texture::LoadTexture(0, L"Resources/2d/enemy.png");
}

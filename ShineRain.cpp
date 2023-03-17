#include "ShineRain.h"
#include "ImageManager.h"

ShineRain::ShineRain() {
}
ShineRain::~ShineRain() {
}

void ShineRain::Init() {
	for (int i = 0; i < 10; i++) {
		Sprite* shine_ = nullptr;
		shine_ = Sprite::Create(ImageManager::kShine, { 0,-10 });
		
		vel = (float)(rand() % 4) + 1.0f;
		
		size = { 20,20 };
		shine_->SetSize(size);
		
		
		shine.reset(shine_);


		shines.push_back(shine_);

	}



}

void ShineRain::Update() {

	for (Shine& shine : shines) {
		XMFLOAT2 pos = shine.sprite->GetPosition();
		pos.y += shine.vel;
		if (pos.y >= 720) {
			pos.x = (float)(rand() % 1280);
			pos.y = max(0, pos.y);
		}
		shine.sprite->SetPosition(pos);
	}
}

void ShineRain::Draw() {
	Sprite::PreDraw();
	for (Shine& shine : shines) {
		shine.sprite->Draw();
	}

}

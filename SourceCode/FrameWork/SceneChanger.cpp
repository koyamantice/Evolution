#include "SceneChanger.h"
#include "ImageManager.h"
#include <SceneManager.h>


SceneChanger::SceneChanger() {
}

SceneChanger::~SceneChanger() {
	for (std::unique_ptr<Sprite>& sprite : sprites) {
		sprite.reset();
	}
	sprites.clear();
}


void SceneChanger::Init() {
	for (int i = 0; i < width_num; i++) {
		for (int j = 0; j < height_num; j++) {
			Sprite* itr = nullptr;
			itr = Sprite::Create(ImageManager::Test, { 0,0 });
			itr->SetAnchorPoint({ 0.5f,0.5f });
			itr->SetPosition({ (float)(i * base_size) + base_size / 2,(float)(j * base_size) + base_size / 2 });
			itr->SetSize({ (float)base_size ,(float)base_size });
			DirectX::XMFLOAT4 col = {0,0,0,1};
			itr->SetColor(col);
			std::unique_ptr<Sprite> newSprite;
			newSprite.reset(itr);

			sprites.push_back(std::move(newSprite));
		}
	}
}


void SceneChanger::Draw() {
	if (!easing_start) { return; }
	for (std::unique_ptr<Sprite>& sprite : sprites) {
		sprite->Draw();
	}
}

void SceneChanger::ChangeScene(const std::string& sceneName) {
	if (easing_start) {
		if (ease_frame >= 1.0f) {
			SceneManager::GetInstance()->ChangeScene(sceneName);
		} else {
			ease_frame += 1.0f / end_frame;
		}
		float rot = 0;
		rot = Ease(InOut, Quint, ease_frame, 0, 360);
		DirectX::XMFLOAT2 size{};
		size.x = Ease(InOut, Quint, ease_frame, 0, (float)base_size);
		size.y = Ease(InOut, Quint, ease_frame, 0, (float)base_size);
		for (std::unique_ptr<Sprite>& sprite : sprites) {
			sprite->SetRotation(rot);
			sprite->SetSize({ size.x,size.y });
		}
	}
}

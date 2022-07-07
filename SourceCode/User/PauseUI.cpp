#include"PauseUI.h"
#include"ImageManager.h"
#include"Easing.h"

PauseUI::PauseUI() {
	Sprite* UI_[Max]{};
	UI_[Sheet] = Sprite::Create(ImageManager::Test, { 0.0f,0.0f });
	UI_[Sheet]->SetAnchorPoint({0.5f,0.5f});
	UI_[Sheet]->SetPosition(pos);
	UI_[Sheet]->SetSize({ 640,360 });
	UI[Sheet].reset(UI_[Sheet]);
	ease = true;
}

PauseUI::~PauseUI() {

}

void PauseUI::Initialize() {

}

void PauseUI::Update() {
	if (ease) {
		pos.y=Ease(In,Quad,frame,-360,360);
		if (frame<1.0f) {
			frame += 0.1f;
		} else {
			ease = false;
		}
		UI[Sheet]->SetPosition(pos);
	}

}

void PauseUI::Draw() {
	Sprite::PreDraw();
	UI[Sheet]->Draw();
}

void PauseUI::Reset() {
	pos = { 640,-360 };
	UI[Sheet]->SetPosition(pos);
	frame = 0;
	ease = true;
}

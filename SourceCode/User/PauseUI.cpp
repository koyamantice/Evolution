#include"PauseUI.h"
#include"ImageManager.h"
#include"Easing.h"

PauseUI::PauseUI() {
	Sprite* UI_[Max]{};

	UI_[Sheet] = Sprite::Create(ImageManager::Test, { 0.0f,0.0f });
	UI_[Sheet]->SetAnchorPoint({ 0.5f,0.5f });
	UI_[Sheet]->SetPosition(pos);
	UI_[Sheet]->SetSize(size);
	UI[Sheet].reset(UI_[Sheet]);
	
	UI_[TitleBack] = Sprite::Create(ImageManager::Test, { 0.0f,0.0f });
	UI_[TitleBack]->SetAnchorPoint({ 0.5f,0.5f });
	UI_[TitleBack]->SetPosition({ pos.x,pos.y - 110 });
	UI_[TitleBack]->SetSize({ 440,80 });
	UI_[TitleBack]->SetColor({1,1,0,1});
	UI[TitleBack].reset(UI_[TitleBack]);
	
	UI_[Option] = Sprite::Create(ImageManager::Test, { 0.0f,0.0f });
	UI_[Option]->SetAnchorPoint({ 0.5f,0.5f });
	UI_[Option]->SetPosition(pos);
	UI_[Option]->SetSize({440,80});
	UI_[Option]->SetColor({ 0,1,1,1 });
	UI[Option].reset(UI_[Option]);

	UI_[ZBack] = Sprite::Create(ImageManager::Test, { 0.0f,0.0f });
	UI_[ZBack]->SetAnchorPoint({ 0.5f,0.5f });
	UI_[ZBack]->SetPosition({ pos.x,pos.y + 110 });
	UI_[ZBack]->SetSize({ 440,80 });
	UI_[ZBack]->SetColor({ 1,0,1,1 });
	UI[ZBack].reset(UI_[ZBack]);
	ease = true;
}

PauseUI::~PauseUI() {

}

void PauseUI::Initialize() {

}

void PauseUI::Update() {
	if (ease) {
		size.x = Ease(In,Quad,frame,0,720);
		size.y = Ease(In, Quad, frame, 0, 360);
		if (frame<1.0f) {
			frame += 0.03f;
		} else {
			ease = false;
		}
		UI[Sheet]->SetSize(size);
	}

}

void PauseUI::Draw() {
	Sprite::PreDraw();
	for (int i = 0; i < Max;i++) {
		UI[i]->Draw();
	}
}

void PauseUI::Reset() {
	pos = { 640,360 };
	UI[Sheet]->SetPosition(pos);
	size = {0,0};
	UI[Sheet]->SetSize(size);
	frame = 0;
	ease = true;
}

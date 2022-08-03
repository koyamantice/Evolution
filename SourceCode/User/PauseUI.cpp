#include"PauseUI.h"
#include"ImageManager.h"
#include"Easing.h"

PauseUI::PauseUI() {
	Sprite* UI_[Max]{};

	UI_[Sheet] = Sprite::Create(ImageManager::UISheet, { 0.0f,0.0f });
	UI_[Sheet]->SetAnchorPoint({ 0.5f,0.5f });
	UI_[Sheet]->SetPosition(pos);
	UI_[Sheet]->SetSize(size[Sheet]);
	UI[Sheet].reset(UI_[Sheet]);
	
	UI_[TitleBack] = Sprite::Create(ImageManager::TitleBack, { 0.0f,0.0f });
	UI_[TitleBack]->SetAnchorPoint({ 0.5f,0.5f });
	UI_[TitleBack]->SetPosition({ pos.x,pos.y - 110 });
	UI_[TitleBack]->SetSize(size[TitleBack]);// { 440, 80 });
	UI[TitleBack].reset(UI_[TitleBack]);
	
	UI_[Option] = Sprite::Create(ImageManager::Option, { 0.0f,0.0f });
	UI_[Option]->SetAnchorPoint({ 0.5f,0.5f });
	UI_[Option]->SetPosition(pos);
	UI_[Option]->SetSize(size[Option]);// ({440,80});
	UI[Option].reset(UI_[Option]);

	UI_[ZBack] = Sprite::Create(ImageManager::ZBack, { 0.0f,0.0f });
	UI_[ZBack]->SetAnchorPoint({ 0.5f,0.5f });
	UI_[ZBack]->SetPosition({ pos.x,pos.y + 110 });
	UI_[ZBack]->SetSize(size[ZBack]);// ({440,80});
	UI[ZBack].reset(UI_[ZBack]);
	ease = true;
}

PauseUI::~PauseUI() {

}

void PauseUI::Initialize() {

}

void PauseUI::Update() {
	if (ease) {
		size[0].x = Ease(InOut,Quad,frame,360,720);
		size[0].y = Ease(InOut, Quad, frame, 180, 360);
		for (int i = 1; i < Max;i++) {
			size[i].x = Ease(InOut, Quad, frame, 0, 440);
			size[i].y = Ease(InOut, Quad, frame, 0, 80);
		}
		if (frame<1.0f) {
			frame += 0.05f;
		} else {
			ease = false;
		}
		for (int i = 0; i < Max; i++) {
			UI[i]->SetSize(size[i]);
		}
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
	for (int i = 0; i < Max; i++) {
		size[i] = {0,0};
		UI[Sheet]->SetSize(size[i]);
	}
	frame = 0;
	ease = true;
}

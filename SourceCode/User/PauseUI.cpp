#include"PauseUI.h"
#include"ImageManager.h"
#include"Easing.h"
#include <Input.h>

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


	UI_[Bar] = Sprite::Create(ImageManager::Bar, { 0.0f,0.0f });
	UI_[Bar]->SetAnchorPoint({ 0.5f,0.5f });
	UI_[Bar]->SetPosition({ pos.x,pos.y - 110 });
	UI_[Bar]->SetSize(size[Bar]);// ({440,80});
	UI[Bar].reset(UI_[Bar]);
	ease = true;
}

PauseUI::~PauseUI() {

}

void PauseUI::Initialize() {

}

void PauseUI::Update() {
	Input* input = Input::GetInstance();

	if (ease) {
		size[0].x = Ease(InOut, Quad, frame, 360, 720);
		size[0].y = Ease(InOut, Quad, frame, 180, 360);
		for (int i = 1; i < Max; i++) {
			size[i].x = Ease(InOut, Quad, frame, 0, 440);
			size[i].y = Ease(InOut, Quad, frame, 0, 80);
		}
		for (int i = 0; i < Max; i++) {
			UI[i]->SetSize(size[i]);
		}
		if (frame < 1.0f) {
			frame += 0.05f;
		} else {
			ease = false;
		}

	}

	if (input->TiltStick(Input::L_UP) || input->TriggerButton(Input::UP) || input->TriggerKey(DIK_UP)) {
		nowBar--;
	}
	if (input->TiltStick(Input::L_DOWN) || input->TriggerButton(Input::DOWN) || input->TriggerKey(DIK_DOWN)) {
		nowBar++;
	}

	if (nowBar < 0) {
		nowBar = 0;
	}
	if (nowBar > 2) {
		nowBar = 2;
	}

	switch (nowBar) {
	case 0:
		SetPos = 360 - 110;
		break;
	case 1:
		SetPos = 360;
		break;
	case 2:
		SetPos = 360 + 110;
		break;
	default:
		break;
	}
	move.y = Ease(In, Quad, 0.7f, UI[Bar]->GetPosition().y, SetPos);
	UI[Bar]->SetPosition({ 640,move.y });
}

void PauseUI::Draw() {
	Sprite::PreDraw();
	for (int i = 0; i < Max; i++) {
		UI[i]->Draw();
	}
}

void PauseUI::Reset() {
	size[Sheet] = { 360,180 };
	UI[Sheet]->SetSize(size[Sheet]);
	for (int i = 1; i < Max; i++) {
		size[i] = { 0,0 };
		UI[i]->SetSize(size[i]);
	}
	frame = 0;
	nowBar = 0;
	ease = true;
}

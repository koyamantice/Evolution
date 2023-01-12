#include"PauseUI.h"
#include"ImageManager.h"
#include"Easing.h"
#include <Input.h>
#include <SceneManager.h>

PauseUI::PauseUI() {
	Sprite* UI_[Max]{};

	UI_[Sheet] = Sprite::Create(ImageManager::UISheet, { 0.0f,0.0f });
	UI_[Sheet]->SetAnchorPoint({ 0.5f,0.5f });
	UI_[Sheet]->SetPosition(pos);
	UI_[Sheet]->SetSize(size[Sheet]);
	UI[Sheet].reset(UI_[Sheet]);

	UI_[kPause] = Sprite::Create(ImageManager::optionline, { 0.0f,0.0f });
	UI_[kPause]->SetAnchorPoint({ 0.5f,0.5f });
	UI_[kPause]->SetPosition({ pos.x,pos.y - 180 });
	UI_[kPause]->SetSize(size[kPause]);
	 UI[kPause].reset(UI_[kPause]);


	UI_[TitleBack] = Sprite::Create(ImageManager::TitleBack, { 0.0f,0.0f });
	UI_[TitleBack]->SetAnchorPoint({ 0.5f,0.5f });
	UI_[TitleBack]->SetPosition({ pos.x,pos.y - 70 });
	UI_[TitleBack]->SetSize(size[TitleBack]);// { 440, 80 });
	UI[TitleBack].reset(UI_[TitleBack]);

	UI_[Option] = Sprite::Create(ImageManager::Option, { 0.0f,0.0f });
	UI_[Option]->SetAnchorPoint({ 0.5f,0.5f });
	UI_[Option]->SetPosition({ pos.x,pos.y + 50 });
	UI_[Option]->SetSize(size[Option]);// ({440,80});
	UI[Option].reset(UI_[Option]);

	UI_[ZBack] = Sprite::Create(ImageManager::ZBack, { 0.0f,0.0f });
	UI_[ZBack]->SetAnchorPoint({ 0.5f,0.5f });
	UI_[ZBack]->SetPosition({ pos.x,pos.y + 170 });
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
	//開始時処理
	FirstOpen();
	//Move処理
	MoveSelect();
	//オプション処理
	OptionSystem();
}

void PauseUI::Draw() {
	Sprite::PreDraw();
	if (!option_system) {
		for (int i = 0; i < Max; i++) {
			UI[i]->Draw();
		}
		UI[kPause]->Draw();
	} else {
		UI[0]->Draw();
	}
}

void PauseUI::Reset() {
	size[Sheet] = { 512,384 };
	UI[Sheet]->SetSize(size[Sheet]);
	for (int i = 1; i < Max; i++) {
		size[i] = { 0,0 };
		UI[i]->SetSize(size[i]);
	}
	frame = 0;
	nowBar = 0;
	ease = true;
}

void PauseUI::FirstOpen() {
	if (!ease) { return; }
		size[0].x = Ease(InOut, Quad, frame, 360, 880);
		size[0].y = Ease(InOut, Quad, frame, 180, 520);
		for (int i = 1; i < Max; i++) {
			size[i].x = Ease(InOut, Quad, frame, 0, 600);
			size[i].y = Ease(InOut, Quad, frame, 0, 96);
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

void PauseUI::MoveSelect() {
	if (ease) { return; }
	if (option_system) { return; }
	Input* input = Input::GetInstance();
	if (input->TiltStick(Input::L_UP) || input->TriggerButton(Input::UP) || input->TriggerKey(DIK_UP)) {
		nowBar--;
	}
	if (input->TiltStick(Input::L_DOWN) || input->TriggerButton(Input::DOWN) || input->TriggerKey(DIK_DOWN)) {
		nowBar++;
	}
	if (input->TriggerButton(input->A) ||input->TriggerKey(DIK_SPACE)) {
			switch (nowBar) {
			case 0:
				SceneManager::GetInstance()->ChangeScene("TITLE");
			case 1:
				option_system = true;
				break;
			case 2:
				Reset();
				endflag = true;
				break;
			default:
				break;
			}
	}

	if (nowBar < 0) { nowBar = 0; }
	if (nowBar > 2) { nowBar = 2; }
	switch (nowBar) {
	case 0:
		SetPos = pos.y - 70;
		break;
	case 1:
		SetPos = pos.y + 50;
		break;
	case 2:
		SetPos = pos.y + 170;
		break;
	default:
		break;
	}
	move.y = Ease(In, Quad, 0.7f, UI[Bar]->GetPosition().y, SetPos);
	UI[Bar]->SetPosition({ 640,move.y });

}

void PauseUI::OptionSystem() {
	if (ease) { return; }
	if (!option_system) { return; }






}

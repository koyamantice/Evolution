#include"PauseUI.h"
#include"ImageManager.h"
#include"Easing.h"
#include <SceneManager.h>


int PauseUI::reverse_camera= NOREVERSE;

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
	UI_[Bar]->SetPosition({ pos.x,pos.y - 70 });
	UI_[Bar]->SetSize(size[Bar]);// ({440,80});
	UI[Bar].reset(UI_[Bar]);


	UI_[CameraOpt] = Sprite::Create(ImageManager::CameraOpt, { 0.0f,0.0f });
	UI_[CameraOpt]->SetAnchorPoint({ 0.5f,0.5f });
	UI_[CameraOpt]->SetPosition({ pos.x,pos.y - 190 });
	UI_[CameraOpt]->SetSize(size[CameraOpt]);// ({440,80});
	UI[CameraOpt].reset(UI_[CameraOpt]);

	 UI_[Normal] = Sprite::Create(ImageManager::Normal, { 0.0f,0.0f });
	 UI_[Normal]->SetAnchorPoint({ 0.5f,0.5f });
	 UI_[Normal]->SetPosition({ pos.x-150,pos.y - 80 });
	 UI_[Normal]->SetSize(size[Normal]);// ({440,80});
	 UI[Normal].reset(UI_[Normal]);

	 UI_[Reverse] = Sprite::Create(ImageManager::Reverse, { 0.0f,0.0f });
	 UI_[Reverse]->SetAnchorPoint({ 0.5f,0.5f });
	 UI_[Reverse]->SetPosition({ pos.x+150,pos.y - 80 });
	 UI_[Reverse]->SetSize(size[Reverse]);// ({440,80});
	 UI[Reverse].reset(UI_[Reverse]);

	 //UI非表示









	  UI_[OptBack] = Sprite::Create(ImageManager::OptionBack, { 0.0f,0.0f });
	  UI_[OptBack]->SetAnchorPoint({ 0.5f,0.5f });
	  UI_[OptBack]->SetPosition({ pos.x,pos.y + 180 });
	  UI_[OptBack]->SetSize(size[OptBack]);// ({440,80});
	  UI[OptBack].reset(UI_[OptBack]);

	  UI_[CameraBar] = Sprite::Create(ImageManager::CameraBar, { 0.0f,0.0f });
	  UI_[CameraBar]->SetAnchorPoint({ 0.5f,0.5f });
	  UI_[CameraBar]->SetPosition({ pos.x - 150,pos.y - 80 });
	  UI_[CameraBar]->SetSize(size[CameraBar]);// ({440,80});
	   UI[CameraBar].reset(UI_[CameraBar]);


	  
	SceneChanger* scene_changer_ = new SceneChanger();
	scene_changer_->Init();
	scene_changer.reset(scene_changer_);

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
		for (int i = 0; i < CameraOpt; i++) {
			UI[i]->Draw();
		}
	} else {
		UI[Sheet]->Draw();
		for (int i = CameraOpt; i < Max; i++) {
			UI[i]->Draw();
		}
	}
	scene_changer->Draw();
}

void PauseUI::Reset() {
	for (int i = 0; i < Max; i++) {
		size[i] = { 0,0 };
		UI[i]->SetSize(size[i]);
	}
	UI[Bar]->SetPosition({ pos.x,pos.y - 70 });
	frame = 0;
	ease = true;
	nowBar = 0;

}

void PauseUI::FirstOpen() {
	if (option_system) { return; }
	if (!ease) { return; }
		size[0].x = Ease(InOut, Quad, frame, 360, 880);
		size[0].y = Ease(InOut, Quad, frame, 180, 520);
		for (int i = 1; i < CameraOpt; i++) {
			size[i].x = Ease(InOut, Quad, frame, 0, 600);
			size[i].y = Ease(InOut, Quad, frame, 0, 96);
		}
		for (int i = 0; i < CameraOpt; i++) {
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
	if(scene_changer->GetEasingStart()){scene_changer->ChangeScene("TITLE"); return;}
	if (input->TiltStick(Input::L_UP) || input->TriggerButton(Input::UP) || input->TriggerKey(DIK_UP)) {
		nowBar--;
	}
	if (input->TiltStick(Input::L_DOWN) || input->TriggerButton(Input::DOWN) || input->TriggerKey(DIK_DOWN)) {
		nowBar++;
	}
	if (input->TriggerButton(input->A) ||input->TriggerKey(DIK_SPACE)) {
			switch (nowBar) {
			case 0:
				scene_changer->ChangeStart();
				return;
				break;
			case 1:
				ease = true;
				frame = 0.0f;
				if (reverse_camera == NOREVERSE) {
					cameraNow = 0;
				} else {
					cameraNow = 1;
				}
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
	if (!option_system) { return; }
	if (ease) {
		size[0].x = Ease(InOut, Quad, frame, 360, 880);
		size[0].y = Ease(InOut, Quad, frame, 180, 520);

		size[CameraOpt].x = Ease(InOut, Quad, frame, 0, 600);
		size[CameraOpt].y = Ease(InOut, Quad, frame, 0, 96);

		for (int i = Normal; i < Max; i++) {
			size[i].x = Ease(InOut, Quad, frame, 0, 280);
			size[i].y = Ease(InOut, Quad, frame, 0, 96);
		}
		UI[0]->SetSize(size[0]);
		for (int i = CameraOpt; i < Max; i++) {
			UI[i]->SetSize(size[i]);
		}
		if (frame < 1.0f) {
			frame += 0.05f;
		} else {
			ease = false;
		}
		return;
	}
	if (input->TiltStick(Input::L_UP) || input->TiltStick(Input::L_LEFT) || input->TriggerKey(DIK_UP)) {
		cameraNow--;
	}
	if (input->TiltStick(Input::L_DOWN) || input->TiltStick(Input::L_RIGHT) || input->TriggerKey(DIK_DOWN)) {
		cameraNow++;
	}
	if (cameraNow < 0) { cameraNow = 0; }
	if (cameraNow > 2) { cameraNow = 2; }

	if (input->TriggerButton(input->A) || input->TriggerKey(DIK_SPACE)) {
		option_system = false;
		ease = true;
		frame = 0.0f;
		nowBar = 1;
		switch (cameraNow) {
		case 0:
			if (reverse_camera == REVERSE) {
				reverse_camera = NOREVERSE;
			}
			break;
		case 1:
			if (reverse_camera == NOREVERSE) {
				reverse_camera = REVERSE;
			}
			break;
		case 2:
			break;
		default:
			break;
		}
		return;
	}

	switch (cameraNow) {
	case 0:
		bar_pos = { pos.x - 150,pos.y - 80 };
		break;
	case 1:
		bar_pos = { pos.x + 150,pos.y - 80 };
		break;
	case 2:
		bar_pos = { pos.x,pos.y + 180 };
		break;
	default:
		break;
	}
	move.x = Ease(In, Quad, 0.7f, UI[CameraBar]->GetPosition().x, bar_pos.x);
	move.y = Ease(In, Quad, 0.7f, UI[CameraBar]->GetPosition().y, bar_pos.y);
	UI[CameraBar]->SetPosition({ move.x,move.y });
}

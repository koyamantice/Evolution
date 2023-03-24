#include"PauseUI.h"
#include"ImageManager.h"
#include"Easing.h"
#include <SceneManager.h>

PauseUI::CameraSystem PauseUI::reverse_camera = CameraSystem::NOREVERSE;

void (PauseUI::* PauseUI::systemFuncTable[])() = {
	&PauseUI::OpenOptionMenu,//要素0
	&PauseUI::SelectOption,
	&PauseUI::OpenCheck2Title,
	&PauseUI::FinalCheck2Title,
	&PauseUI::OpenChangeManual,
	&PauseUI::ChangeManualSystem,
	&PauseUI::CloseOptionMenu,
};




PauseUI::PauseUI() {
	//basesizes
	const XMFLOAT2 baseAnchor = { 0.5f,0.5f };

	for (int i = 0; i < Max; i++) {
		UI[i] = Sprite::Create(ImageManager::UISheet + i, basePos);
		UI[i]->SetAnchorPoint(baseAnchor);
		basesizes[i] = UI[i]->GetSize();
		UI[i]->SetSize({ 0,0 });
	}

	Initialize();

	SceneChanger* scene_changer_ = new SceneChanger();
	scene_changer_->Init();
	sceneChanger_.reset(scene_changer_);

}

PauseUI::~PauseUI() {

}

void PauseUI::Initialize() {
	UI[Sheet]->SetPosition(pos);
	UI[kPause]->SetPosition({ pos.x,pos.y - 180 });

	UI[TitleBack]->SetPosition({ basePos.x,bar_pos_[0] });
	UI[Option]->SetPosition({ basePos.x,bar_pos_[1] });
	UI[ZBack]->SetPosition({ basePos.x,bar_pos_[2] });

	UI[Bar]->SetPosition({ basePos.x,bar_pos_[static_cast<size_t>(frameBar_)] });


	//カメラオプション
	UI[CameraOpt]->SetPosition({ pos.x,pos.y - 190 });

	UI[Normal]->SetPosition(manual_pos_[static_cast<size_t>(ManualFrame::NOREVERSEBUTTON)]);
	UI[Reverse]->SetPosition(manual_pos_[static_cast<size_t>(ManualFrame::REVERSEBUTTON)]);
	UI[OptBack]->SetPosition(manual_pos_[static_cast<size_t>(ManualFrame::BACKSELECT)]);
	UI[CameraBar]->SetPosition(manual_pos_[static_cast<size_t>(ManualFrame::NOREVERSEBUTTON)]);

	UI[kOptionConfig]->SetPosition({ pos.x,pos.y - 190 });
	UI[kTitleOk]->SetPosition({ pos.x - 150,pos.y + 180 });
	UI[kTitleCancel]->SetPosition({ pos.x + 150,pos.y + 180 });
	UI[ConfigBar]->SetPosition({ pos.x + 150,pos.y + 180 });

}

void PauseUI::Update() {
	if (sceneChanger_->GetEasingStart()) { sceneChanger_->ChangeScene("TITLE", SceneChanger::Reverse); return; }

	if (input->TriggerButton(input->START)) {
		Reset();
		state_ = SystemConfig::kOpenOptionMenu;
		endflag = true;
	}

	//関数ポインタで状態管理
	(this->*systemFuncTable[static_cast<size_t>(state_)])();
}

void PauseUI::Draw() {
	Sprite::PreDraw();
	switch (state_) {
	case SystemConfig::kOpenOptionMenu:
		for (int i = Sheet; i <= Bar; i++) {
			UI[i]->Draw();
		}

		break;
	case SystemConfig::kSelectOption:
		for (int i = Sheet; i < CameraOpt; i++) {
			UI[i]->Draw();
		}

		break;
	case SystemConfig::kOpenCheck2Title:
		UI[Sheet]->Draw();
		for (int i = kOptionConfig; i < Max; i++) {
			UI[i]->Draw();
		}
		break;
	case SystemConfig::kFinalCheck2Title:
		UI[Sheet]->Draw();
		for (int i = kOptionConfig; i < Max; i++) {
			UI[i]->Draw();
		}
		break;
	case SystemConfig::kOpenChangeManual:
		UI[Sheet]->Draw();
		for (int i = CameraOpt; i <= CameraBar; i++) {
			UI[i]->Draw();
		}
		break;
	case SystemConfig::kChangeManualSystem:
		UI[Sheet]->Draw();
		for (int i = CameraOpt; i <= CameraBar; i++) {
			UI[i]->Draw();
		}
		break;
	case SystemConfig::kCloseOptionMenu:
		for (int i = Sheet; i <= Bar; i++) {
			UI[i]->Draw();
		}
		break;
	default:
		break;
	}
	sceneChanger_->Draw();
}


void PauseUI::OpenOptionMenu() {

	open_frame_ += 1.0f / 15.0f;

	if (open_frame_ >= 1.0f) {
		open_frame_ = max(1.0f, open_frame_);
		state_ = SystemConfig::kSelectOption;
	}

	sizes[Sheet].x = Ease(InOut, Quad, open_frame_, basesizes[Sheet].x / 2.0f, basesizes[Sheet].x);
	sizes[Sheet].y = Ease(InOut, Quad, open_frame_, basesizes[Sheet].y / 2.0f, basesizes[Sheet].y);

	for (int i = Sheet; i <= Bar; i++) {
		sizes[i].x = Ease(InOut, Quad, open_frame_, 0, basesizes[i].x);
		sizes[i].y = Ease(InOut, Quad, open_frame_, 0, basesizes[i].y);
	}

	for (int i = Sheet; i <= Bar; i++) {
		UI[i]->SetSize(sizes[i]);
	}

}

void PauseUI::SelectOption() {
	int nowBar_ = (int)frameBar_;
	if (input->TiltStick(Input::L_UP) || input->TriggerButton(Input::UP) || input->TriggerKey(DIK_UP) ||
		input->TiltStick(Input::L_RIGHT) || input->TriggerButton(Input::RIGHT) || input->TriggerKey(DIK_RIGHT)) {
		nowBar_--;
	}
	if (input->TiltStick(Input::L_DOWN) || input->TriggerButton(Input::DOWN) || input->TriggerKey(DIK_DOWN) ||
		input->TiltStick(Input::L_LEFT) || input->TriggerButton(Input::LEFT) || input->TriggerKey(DIK_LEFT)) {
		nowBar_++;
	}
	nowBar_ = min(nowBar_, (int)SelectFrame::CloseOptionMenu);
	nowBar_ = max(nowBar_, (int)SelectFrame::Back2Title);

	frameBar_ = static_cast<SelectFrame>(nowBar_);

	if (input->TriggerButton(input->A) || input->TriggerKey(DIK_SPACE)) {	
		switch (frameBar_) {
		case SelectFrame::Back2Title:
			state_ = SystemConfig::kOpenCheck2Title;
			open_frame_ = 0.0f;
			break;
		case SelectFrame::ChangeManualSystem:
			state_ = SystemConfig::kOpenChangeManual;
			if (reverse_camera == CameraSystem::NOREVERSE) {
				manualBar_ = ManualFrame::NOREVERSEBUTTON;
			} else {
				manualBar_ = ManualFrame::REVERSEBUTTON;;
			}
			UI[CameraBar]->SetPosition(manual_pos_[static_cast<size_t>(manualBar_)]);
			break;
		case SelectFrame::CloseOptionMenu:
			state_ = SystemConfig::kCloseOptionMenu;
			break;
		default:
			break;
		}

	}


	move.y = Ease(In, Quad, 0.7f, UI[Bar]->GetPosition().y, bar_pos_[static_cast<size_t>(frameBar_)]);
	UI[Bar]->SetPosition({ basePos.x,move.y });

}

void PauseUI::OpenCheck2Title() {

	open_frame_ += 1.0f / 15.0f;

	if (open_frame_ >= 1.0f) {
		open_frame_ = max(1.0f, open_frame_);
		state_ = SystemConfig::kFinalCheck2Title;
	}

	sizes[Sheet].x = Ease(InOut, Quad, open_frame_, basesizes[Sheet].x / 2.0f, basesizes[Sheet].x);
	sizes[Sheet].y = Ease(InOut, Quad, open_frame_, basesizes[Sheet].y / 2.0f, basesizes[Sheet].y);
	UI[Sheet]->SetSize(sizes[Sheet]);

	for (int i = kOptionConfig; i < Max; i++) {
		sizes[i].x = Ease(InOut, Quad, open_frame_, 0, basesizes[i].x);
		sizes[i].y = Ease(InOut, Quad, open_frame_, 0, basesizes[i].y);
		UI[i]->SetSize(sizes[i]);

	}
}

void PauseUI::FinalCheck2Title() {
	int nowBar_ = (int)finalCheckBar_;
	if (input->TiltStick(Input::L_UP) || input->TriggerButton(Input::UP) || input->TriggerKey(DIK_UP)||
		input->TiltStick(Input::L_RIGHT) || input->TriggerButton(Input::RIGHT) || input->TriggerKey(DIK_RIGHT)) {
		nowBar_--;
	}
	if (input->TiltStick(Input::L_DOWN) || input->TriggerButton(Input::DOWN) || input->TriggerKey(DIK_DOWN)||
		input->TiltStick(Input::L_LEFT) || input->TriggerButton(Input::LEFT) || input->TriggerKey(DIK_LEFT)) {
		nowBar_++;
	}
	if (nowBar_ < (int)FinalCheckFrame::BACKTITLE) { nowBar_ = (int)FinalCheckFrame::CANCELSELECT; }
	if (nowBar_ > (int)FinalCheckFrame::CANCELSELECT) { nowBar_ = (int)FinalCheckFrame::BACKTITLE; }
	finalCheckBar_ = static_cast<FinalCheckFrame>(nowBar_);

	if (input->TriggerButton(input->A) || input->TriggerKey(DIK_SPACE)) {
		Reset();
		switch (finalCheckBar_) {
		case FinalCheckFrame::BACKTITLE:
			sceneChanger_->ChangeStart();
			break;
		case FinalCheckFrame::CANCELSELECT:
			state_ = SystemConfig::kOpenOptionMenu;
			break;
		default:
			break;
		}
		return;
	}

	move.x = Ease(In, Quad, 0.7f, UI[ConfigBar]->GetPosition().x, final_pos_[static_cast<size_t>(finalCheckBar_)].x);
	move.y = Ease(In, Quad, 0.7f, UI[ConfigBar]->GetPosition().y, final_pos_[static_cast<size_t>(finalCheckBar_)].y);
	UI[ConfigBar]->SetPosition({ move.x,move.y });
}

void PauseUI::OpenChangeManual() {
	frame += 1.0f / 20.0f;

	if (frame >= 1.0f) {
		frame = max(1.0f, frame);
		state_ = SystemConfig::kChangeManualSystem;
	}

	for (int i = CameraOpt; i < Max; i++) {
		sizes[i].x = Ease(InOut, Quad, frame, 0, basesizes[i].x);
		sizes[i].y = Ease(InOut, Quad, frame, 0, basesizes[i].y);
		UI[i]->SetSize(sizes[i]);
	}
}

void PauseUI::ChangeManualSystem() {
	int nowBar_ = (int)manualBar_;
	if (input->TiltStick(Input::L_UP) || input->TriggerButton(Input::UP) || input->TriggerKey(DIK_UP) ||
		input->TiltStick(Input::L_RIGHT) || input->TriggerButton(Input::RIGHT) || input->TriggerKey(DIK_RIGHT)) {
		nowBar_--;
	}
	if (input->TiltStick(Input::L_DOWN) || input->TriggerButton(Input::DOWN) || input->TriggerKey(DIK_DOWN) ||
		input->TiltStick(Input::L_LEFT) || input->TriggerButton(Input::LEFT) || input->TriggerKey(DIK_LEFT)) {
		nowBar_++;
	}
	if (nowBar_ < (int)ManualFrame::NOREVERSEBUTTON) {nowBar_ = (int)ManualFrame::BACKSELECT;	}
	if (nowBar_ > (int)ManualFrame::BACKSELECT) { nowBar_ = (int)ManualFrame::NOREVERSEBUTTON; }
	manualBar_ = static_cast<ManualFrame>(nowBar_);

	if (input->TriggerButton(input->A) || input->TriggerKey(DIK_SPACE)) {
		state_ = SystemConfig::kOpenOptionMenu;
		Reset();
		switch (manualBar_) {
		case ManualFrame::NOREVERSEBUTTON:
			if (reverse_camera == CameraSystem::REVERSE) {
				reverse_camera = CameraSystem::NOREVERSE;
			}
			break;
		case ManualFrame::REVERSEBUTTON:
			if (reverse_camera == CameraSystem::NOREVERSE) {
				reverse_camera = CameraSystem::REVERSE;
			}
			break;
		case ManualFrame::BACKSELECT:
			break;
		default:
			break;
		}
		return;
	}

	move.x = Ease(In, Quad, 0.7f, UI[CameraBar]->GetPosition().x, manual_pos_[static_cast<size_t>(manualBar_)].x);
	move.y = Ease(In, Quad, 0.7f, UI[CameraBar]->GetPosition().y, manual_pos_[static_cast<size_t>(manualBar_)].y);
	UI[CameraBar]->SetPosition({ move.x,move.y });

}

void PauseUI::CloseOptionMenu() {

	open_frame_ -= 1.0f / 20.0f;

	for (int i = 0; i < Max; i++) {
		sizes[i].x = Ease(InOut, Quint, open_frame_, 0, basesizes[i].x);
		sizes[i].y = Ease(InOut, Quint, open_frame_, 0, basesizes[i].y);
		UI[i]->SetSize(sizes[i]);
	}

	if (open_frame_ <= 0.0f) {
		open_frame_ = max(open_frame_, 0.0f);
		Reset();
		state_ = SystemConfig::kOpenOptionMenu;
		endflag = true;
	}
}

void PauseUI::Reset() {
	for (int i = 0; i < Max; i++) {
		sizes[i] = { 0,0 };
		UI[i]->SetSize(sizes[i]);
	}
	frameBar_ = SelectFrame::Back2Title;
	UI[Bar]->SetPosition({ basePos.x,bar_pos_[static_cast<size_t>(frameBar_)] });
	frame = 0;
	open_frame_ = 0;
}
#include"PauseUI.h"
#include"ImageManager.h"
#include"Easing.h"
#include <SceneManager.h>

PauseUI::CameraSystem PauseUI::reverse_camera = CameraSystem::NOREVERSE;

void (PauseUI::* PauseUI::systemFuncTable[])() = {
	&PauseUI::OpenOptionMenu,//要素0
	&PauseUI::SelectOption,
	&PauseUI::FinalCheck2Title,
	&PauseUI::OpenChangeManual,
	&PauseUI::ChangeManualSystem,
	&PauseUI::CloseOptionMenu,
};




PauseUI::PauseUI() {
	//basesizes
	const XMFLOAT2 baseAnchor = { 0.5f,0.5f };

	for (int i = 0; i < Max; i++) {
		Sprite* UI_{};

		UI_ = Sprite::Create(ImageManager::UISheet + i, basePos);
		UI_->SetAnchorPoint(baseAnchor);
		basesizes[i] = UI_->GetSize();
		UI_->SetSize({ 0,0 });
		UI[i].reset(UI_);
	}

	Initialize();

	SceneChanger* scene_changer_ = new SceneChanger();
	scene_changer_->Init();
	scene_changer.reset(scene_changer_);

}

PauseUI::~PauseUI() {

}

void PauseUI::Initialize() {
	UI[Sheet]->SetPosition(pos);
	UI[kPause]->SetPosition({ pos.x,pos.y - 180 });

	UI[TitleBack]->SetPosition({ basePos.x,bar_pos_[0]});
	UI[Option]->SetPosition({ basePos.x,bar_pos_[1]});
	UI[ZBack]->SetPosition({ basePos.x,bar_pos_[2]});

	UI[Bar]->SetPosition({ basePos.x,bar_pos_[static_cast<size_t>(FrameBar_)]});


	//カメラオプション
	UI[CameraOpt]->SetPosition({ pos.x,pos.y - 190 });
	UI[Normal]->SetPosition({ pos.x - 150,pos.y - 80 });
	UI[Reverse]->SetPosition({ pos.x + 150,pos.y - 80 });

	UI[CameraBar]->SetPosition({ pos.x - 150,pos.y - 80 });
	UI[OptBack]->SetPosition({ pos.x,pos.y + 180 });
	UI[kOptionConfig]->SetPosition({ pos.x,pos.y + 180 });
	UI[kTitleOk]->SetPosition({ pos.x - 150,pos.y - 80 });

}

void PauseUI::Update() {
	if (input->TriggerButton(input->START)) {
		Reset();
		state_ = SystemConfig::kOpenOptionMenu;
		endflag = true;

	}
	//関数ポインタで状態管理
	(this->*systemFuncTable[static_cast<size_t>(state_)])();

	////開始時処理
	//FirstOpen();
	////Move処理
	//MoveSelect();
	////オプション処理
	//OptionSystem();
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
	case SystemConfig::kFinalCheck2Title:

		break;
	case SystemConfig::kOpenChangeManual:
		UI[Sheet]->Draw();
		for (int i = CameraOpt; i <= OptBack; i++) {
			UI[i]->Draw();
		}
		break;
	case SystemConfig::kChangeManualSystem:
		UI[Sheet]->Draw();
		for (int i = CameraOpt; i <= OptBack; i++) {
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
	scene_changer->Draw();
}


void PauseUI::OpenOptionMenu() {

	open_frame_ += 1.0f / 15.0f;

	if (open_frame_ >= 1.0f) {
		open_frame_ = max(1.0f, open_frame_);
		state_ = SystemConfig::kSelectOption;
	}

	sizes[Sheet].x = Ease(InOut, Quad, open_frame_, basesizes[Sheet].x/2.0f,basesizes[Sheet].x);
	sizes[Sheet].y = Ease(InOut, Quad, open_frame_, basesizes[Sheet].y/2.0f,basesizes[Sheet].y);

	for (int i = Sheet; i <= Bar; i++) {
		sizes[i].x = Ease(InOut, Quad, open_frame_, 0, basesizes[i].x);
		sizes[i].y = Ease(InOut, Quad, open_frame_, 0, basesizes[i].y);
	}

	for (int i = Sheet; i <= Bar; i++) {
		UI[i]->SetSize(sizes[i]);
	}

}

void PauseUI::SelectOption() {
	int nowBar_=(int)FrameBar_;
	if (input->TiltStick(Input::L_UP) || input->TriggerButton(Input::UP) || input->TriggerKey(DIK_UP)) {
		nowBar_--;
	}
	if (input->TiltStick(Input::L_DOWN) || input->TriggerButton(Input::DOWN) || input->TriggerKey(DIK_DOWN)) {
		nowBar_++;
	}
	nowBar_ = min(nowBar_,(int)SelectFrame::CloseOptionMenu);
	nowBar_ = max(nowBar_, (int)SelectFrame::Back2Title);

	FrameBar_ = static_cast<SelectFrame>(nowBar_);

	if (input->TriggerButton(input->A) || input->TriggerKey(DIK_SPACE)) {
		switch (FrameBar_) {
		case SelectFrame::Back2Title:
			state_ = SystemConfig::kFinalCheck2Title;
			break;
		case SelectFrame::ChangeManualSystem:
			state_ = SystemConfig::kOpenChangeManual;
			if (reverse_camera == CameraSystem::NOREVERSE) {
				cameraNow = 0;
			} else {
				cameraNow = 1;
			}

			break;
		case SelectFrame::CloseOptionMenu:
			state_ = SystemConfig::kCloseOptionMenu;
			break;
		default:
			break;
		}
	}
	

	move.y = Ease(In, Quad, 0.7f, UI[Bar]->GetPosition().y, bar_pos_[static_cast<size_t>(FrameBar_)]);
	UI[Bar]->SetPosition({ basePos.x,move.y });

}

void PauseUI::FinalCheck2Title() {

	if (scene_changer->GetEasingStart()) { scene_changer->ChangeScene("TITLE", SceneChanger::Reverse); return; }

	scene_changer->ChangeStart();

}

void PauseUI::OpenChangeManual() {
	frame += 1.0f / 20.0f;

	if (frame >= 1.0f) {
		frame = max(1.0f, frame);
		state_ = SystemConfig::kChangeManualSystem;
	}

	for (int i = Sheet; i < Max; i++) {
		sizes[i].x = Ease(InOut, Quad, frame, 0, basesizes[i].x);
		sizes[i].y = Ease(InOut, Quad, frame, 0, basesizes[i].y);
		UI[i]->SetSize(sizes[i]);
	}
}

void PauseUI::ChangeManualSystem() {

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
	FrameBar_ = SelectFrame::Back2Title;
	UI[Bar]->SetPosition({ basePos.x,bar_pos_[static_cast<size_t>(FrameBar_)] });
	frame = 0;
	//ease = true;
}
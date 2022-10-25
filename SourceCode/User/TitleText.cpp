#include"TitleText.h"
#include <SourceCode/User/ModelManager.h>
#include"Easing.h"




TitleText::TitleText() {
}

TitleText::~TitleText() {
}

void TitleText::Init() {

	for (int i = 0; i < 6; i++) {
		Object3d* newText_ = new Object3d();
		if (i == 0) {
			newText_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::textG));
		} else if (i == 1) {
			newText_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::textN));
		} else if (i == 2) {
			newText_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::textO));
		} else if (i == 3) {
			newText_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::textM));
		} else if (i == 4) {
			newText_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::textE));
		} else {
			newText_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::textN));
		}
		newText_->Initialize();
		newText_->SetPosition({ (i - 3) * 6.0f,0,abs((i - 3) * 5.0f) });
		newText_->SetRotation({ 90,0,0 });
		newText_->SetScale({ 5.0f,5.0f,5.0f });
		texts[i].reset(newText_);
	}
	for (int i = 0; i < 6; i++) {
		texts[i]->Initialize();
	}

	FBXObject3d* Player_ = new FBXObject3d();
	Player_->SetModel(ModelManager::GetIns()->GetFBXModel(ModelManager::Bird));
	Player_->Initialize();
	Player_->SetScale({ 0.005f,0.005f, 0.005f });
	Player.reset(Player_);
	Player->SetPosition({-5.0f,-5.0f,0.0f});
}

void TitleText::Upda() {
	for (int i = 0; i < 6; i++) {
		texts[i]->Update();
	}

	frame += 0.01f;

	if (frame < 1.0f) {
		pos[0]=Ease(In,Quad,frame,0,-14.0f);
		pos[1]=Ease(In,Quad,frame,0,-9.0f);
		pos[2]=Ease(In,Quad,frame,0,-4.0f);
		pos[3]=Ease(In,Quad,frame,0,2.0f);
		pos[4]=Ease(In,Quad,frame,0,8.0f);
		pos[5]=Ease(In,Quad,frame,0,14.0f);
	} else {
		frame = 1.0f;
	}

	for (int i = 0; i < 6; i++) {
		texts[i]->SetPosition({ pos[i],5,i*2.0f });
	}

	Player->Update();
}

void TitleText::Draw(DirectXCommon* dxCommon) {
//	Player->Draw(dxCommon->GetCmdList());
	for (int i = 0; i < 6; i++) {
		texts[i]->Draw();
	}

}

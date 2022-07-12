#include "PlayScene.h"
#include"SceneManager.h"
#include "AudioManager.h"
#include "input.h"
#include<string>
#include "DebugText.h"
#include"ImageManager.h"
#include"ModelManager.h"
#include <TisGame.h>
#include"Player.h"
#include"Enemy.h"
#include <SourceCode/Common/Easing.h>

void PlayScene::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	//背景スプライト生成
	//sprite[back] = Sprite::Create(ImageManager::TITLE, { 0.0f,0.0f });
	//スプライト生成
	Actor* Act_[Chr_Max]{};
	Act_[MPlayer] = new Player();
	Act_[MPlayer]->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Player));
	actor[MPlayer].reset(Act_[MPlayer]);

	Act_[MEnemy] = new Enemy();
	Act_[MEnemy]->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Enemy));
	actor[MEnemy].reset(Act_[MEnemy]);


	TouchableObject* Sky{};
	Sky = new TouchableObject();
	//Sky->SetModel(ModelManager::GetIns()->GetModel(ModelManager::skydome));
	Sky->Initialize(ModelManager::GetIns()->GetModel(ModelManager::skydome));
	skydome.reset(Sky);

	TouchableObject* Ground{};
	Ground = new TouchableObject();
	Ground->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Ground));
	ground.reset(Ground);


	PauseUI* pause_ui = new PauseUI();
	pauseUi.reset(pause_ui);
	camera->SetTarget(actor[MPlayer]->GetPosition());
}
//開放処理
void PlayScene::Finalize() {
	//３ｄのモデルのデリート
}
//更新
void PlayScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	CameraUpda();
	if (input->TriggerButton(input->Button_Y)) {
		SceneManager::GetInstance()->ChangeScene("DEBUG");
	}
	if (pause) {
		pauseUi->Update();
		if (input->TriggerButton(input->Start)) {
			pauseUi->Reset();
			pause = false;
		}
		return;
	}
	if (input->TriggerButton(input->Start)||
		input->TriggerKey(DIK_1)) {
		pause = true;
	}
	for (int i = 0; i < Chr_Max; i++) {
		actor[i]->Update();
	}
	skydome->Update();
	ground->Update();
	if (input->PushKey(DIK_0)) {
		int a = 0;
		a++;
	}

}
//描画
void PlayScene::Draw(DirectXCommon* dxCommon) {
	XMFLOAT3 rot= actor[MPlayer]->GetRotation();
	ImGui::Begin("camera");
	ImGui::SliderFloat("rotation", &angle, -360, 360);
	ImGui::SliderFloat("rotation.x", &distance.x, -360, 360);
	ImGui::SliderFloat("rotation.y", &distance.y, -360, 360);
	ImGui::SliderFloat("rot", &dis.x, -360, 360);
	ImGui::SliderFloat("rot", &dis.y, -360, 360);
	ImGui::SliderFloat("rot", &rot.y, -360, 360);
	ImGui::End();
	Object3d::PreDraw();
	skydome->Draw();
	ground->Draw();
	//背景用
	for (int i = 0; i < Chr_Max;i++) {
		actor[i]->Draw();
	}
	if (pause) {
		pauseUi->Draw();
	}
}

void PlayScene::CameraUpda() {
	Input* input = Input::GetInstance();
	XMFLOAT3 plaPos = actor[MPlayer]->GetPosition();
if((input->PushButton(input->Button_RB))|| (input->PushButton(input->Button_LB))
	||input->PushKey(DIK_RIGHT)||input->PushKey(DIK_LEFT)){
	if (input->PushButton(input->Button_RB) || input->PushKey(DIK_RIGHT)) {
		angle-=1;
	}
	if (input->PushButton(input->Button_LB) || input->PushKey(DIK_LEFT)) {
		angle+=1;
	}

	if ((input->PushButton(input->Button_RB)&& (input->PushButton(input->Button_LB)))
		||(input->PushKey(DIK_RIGHT) && input->PushKey(DIK_LEFT))) {
		angle = actor[MPlayer]->GetRotation().y;
	}

	actor[MPlayer]->SetAngle(angle);
	dis.x=sinf(angle * (PI / 180)) * 15.0f;
	dis.y=cosf(angle*(PI/180)) * 15.0f;
}

distance.x=Ease(In,Quad,0.5f,distance.x,dis.x);
distance.y=Ease(In,Quad,0.5f,distance.y,dis.y);

	camera->SetTarget(XMFLOAT3{ plaPos.x,plaPos.y,plaPos.z });
	camera->SetEye(XMFLOAT3{ plaPos.x+distance.x,plaPos.y + 10.0f,plaPos.z + distance.y });
	camera->Update();
}


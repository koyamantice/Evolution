#include "DebugScene.h"
#include"SceneManager.h"
#include "AudioManager.h"
#include "input.h"
#include<string>
#include "DebugText.h"
#include"ImageManager.h"
#include"ModelManager.h"
#include <TisGame.h>
#include"Player.h"
void DebugScene::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	//背景スプライト生成
	//sprite[back] = Sprite::Create(ImageManager::TITLE, { 0.0f,0.0f });
	//スプライト生成
	Actor* Act_[Chr_Max]{};
	Act_[MPlayer] = new Player();
	Act_[MPlayer]->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Player));
	rot = { 0,180,0 };
	Act_[MPlayer]->SetRotation(rot);
	actor[MPlayer].reset(Act_[MPlayer]);
	Act_[Enemy] = new Player();
	Act_[Enemy]->Initialize(ModelManager::GetIns()->GetModel(ModelManager::hole));
	rot = { 0,180,0 };
	Act_[Enemy]->SetRotation(rot);
	actor[Enemy].reset(Act_[Enemy]);
	camera->SetEye({0, 0, -10});
	camera->SetTarget(actor[MPlayer]->GetPosition());
}
//開放処理
void DebugScene::Finalize() {
	//３ｄのモデルのデリート
}
//更新
void DebugScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	XMFLOAT3 plaPos = actor[Select]->GetPosition();
	camera->SetTarget(XMFLOAT3{plaPos.x,plaPos.y,plaPos.z});
	camera->SetEye(XMFLOAT3{ plaPos.x,plaPos.y,plaPos.z+10.0f});
	camera->Update();
	if (input->TriggerKey(DIK_A)) {
		if (Select==1) {
			Select--;
		}
	}
	if (input->TriggerKey(DIK_D)) {
		if (Select == 0) {
			Select++;
		}
	}
	actor[Select]->SetRotation(rot);
	actor[Select]->Update();
}
//描画
void DebugScene::Draw(DirectXCommon* dxCommon) {
	//mGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	//ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(980, 0));
	ImGui::SetNextWindowSize(ImVec2(280, 300));
	ImGui::Begin("test");
	ImGui::SliderFloat("rotation.x", &rot.x, 0, 360);
	ImGui::SliderFloat("rotation.y", &rot.y, 0, 360);
	ImGui::SliderFloat("rotation.z", &rot.z, 0, 360);
	ImGui::Unindent();
	ImGui::End();
	if (Select==0) {
		ImGui::SetNextWindowPos(ImVec2(980, 300));
		ImGui::SetNextWindowSize(ImVec2(280, 300));
		ImGui::Begin("parameter");
		//ImGui::SliderFloat("cameraPos.y", &cameraPos.y, 30, 0);
		ImGui::Unindent();
		ImGui::End();
	}	if (Select == 1) {
		ImGui::SetNextWindowPos(ImVec2(980, 300));
		ImGui::SetNextWindowSize(ImVec2(280, 300));
		ImGui::Begin("enemyparameter");
		//ImGui::SliderFloat("cameraPos.y", &cameraPos.y, 30, 0);
		ImGui::Unindent();
		ImGui::End();
	}
	//Sprite::PreDraw();
	//背景用
	actor[Select]->Draw();
}


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
#include"Enemy.h"

#include <string>
#include <fstream>
#include <sstream>

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

	Act_[MBullet] = new Bullet();
	Act_[MBullet]->Initialize(ModelManager::GetIns()->GetModel(ModelManager::hole));
	rot = { 0,180,0 };
	Act_[MBullet]->SetRotation(rot);
	actor[MBullet].reset(Act_[MBullet]);

	Act_[MEnemy] = new Enemy();
	Act_[MEnemy]->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Enemy));
	rot = { 0,180,0 };
	Act_[MEnemy]->SetRotation(rot);
	actor[MEnemy].reset(Act_[MEnemy]);
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
	if (save) {
		if (Select==0) {
			Save();
		}
		if (Select == 2) {
			EnemySave();
		}
		save = false;
	}
	if (input->TriggerKey(DIK_A)) {
		if (Select != 0) {
			Select--;
		}
	}
	if (input->TriggerKey(DIK_D)) {
		if (Select != 2) {
			Select++;
		}
	}
	if (input->TriggerButton(input->Select)) {
		SceneManager::GetInstance()->ChangeScene("PLAY");
	}
	actor[Select]->SetRotation(rot);
	actor[Select]->Update();
}
//描画
void DebugScene::Draw(DirectXCommon* dxCommon) {
	//mGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
	//ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(980, 0),1);
	ImGui::SetNextWindowSize(ImVec2(280, 300),1);
	ImGui::Begin("test");
	ImGui::SliderFloat("rotation.x", &rot.x, 0, 360);
	ImGui::SliderFloat("rotation.y", &rot.y, 0, 360);
	ImGui::SliderFloat("rotation.z", &rot.z, 0, 360);
	ImGui::Unindent();
	ImGui::End();
	if (Select==0) {
		ImGui::SetNextWindowPos(ImVec2(1000, 300));
		ImGui::SetNextWindowSize(ImVec2(350, 300));
		ImGui::Begin("parameter");
		ImGui::SliderInt("hp", &hp, 0, 360);
		ImGui::SliderFloat("vel", &vel, 0, 5);
		if (ImGui::Button("SAVE", ImVec2(50, 40))) {
			save = true;
		}
		if (ImGui::Button("PlayScene", ImVec2(100, 30))) {
			SceneManager::GetInstance()->ChangeScene("PLAY");
		}
		if (ImGui::Button("TitleScene", ImVec2(100, 30))) {
			SceneManager::GetInstance()->ChangeScene("TITLE");
		}

		ImGui::Unindent();
		ImGui::End();
	}	
	if (Select == 1) {
		ImGui::SetNextWindowPos(ImVec2(980, 300));
		ImGui::SetNextWindowSize(ImVec2(280, 300));
		ImGui::Begin("bulletParameter");
		//ImGui::SliderFloat("cameraPos.y", &cameraPos.y, 30, 0);
		ImGui::Unindent();
		ImGui::End();
	}
	if (Select == 2) {
		ImGui::SetNextWindowPos(ImVec2(1000, 300));
		ImGui::SetNextWindowSize(ImVec2(350, 300));
		ImGui::ShowDemoWindow();
		ImGui::Begin("parameter");
		ImGui::SliderInt("hp", &hp, 0, 360);
		ImGui::SliderFloat("vel", &vel, 0, 5);
		ImGui::InputFloat("waitTimer", &waitTimer, 0, 360);
		//ImGui::
		ImGui::RadioButton("Approch", &phase, Approch);
		ImGui::SameLine();
		ImGui::RadioButton("Leave", &phase, Leave);
		ImGui::SameLine(); 
		ImGui::RadioButton("Wait", &phase, Wait);

		if (ImGui::Button("SAVE", ImVec2(50, 40))) {
			save = true;
		}
		if (ImGui::Button("PlayScene", ImVec2(100, 30))) {
			SceneManager::GetInstance()->ChangeScene("PLAY");
		}
		if (ImGui::Button("TitleScene", ImVec2(100, 30))) {
			SceneManager::GetInstance()->ChangeScene("TITLE");
		}

		ImGui::Unindent();
		ImGui::End();
	}

	//Sprite::PreDraw();
	//背景用
	actor[Select]->Draw();
}

void DebugScene::Save() {
	std::ofstream file("Resources/csv/status.csv");
	file << "//体力" << std::endl;
	file << "HP" << ',';
	file << hp << ',';
	file << std::endl;
	file << "//速度" << std::endl;
	file << "VEL" << ',';
	file << vel << ',';
	file << std::endl;
	file.close();
}

void DebugScene::EnemySave() {
	std::ofstream file("Resources/csv/EnemyStatus.csv");
	file << "//体力" << std::endl;
	file << "HP" << ',';
	file << hp << ',';
	file << std::endl;
	file << "//基本速さ" << std::endl;
	file << "VEL" << ',';
	file << vel << ',';
	file << std::endl;
	file << "//最初のフェーズ" << std::endl;
	file << "PHASE" << ',';
	if (phase==Approch) {
		file << "APPROCH" << ',';
	}
	if (phase == Leave) {
		file << "LEAVE" << ',';
	}
	if (phase == Wait) {
		file << "WAIT" << ',';
	}
	file << std::endl;
	file << "//待機時間" << std::endl;
	file << "WAITTIMER" << ',';
	file << waitTimer << ',';
	file << std::endl;
	file.close();
}


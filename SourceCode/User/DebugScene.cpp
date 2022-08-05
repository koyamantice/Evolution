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
#include <SourceCode/FrameWork/ActorManager.h>

void DebugScene::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	//背景スプライト生成
	Sprite* back_ = Sprite::Create(ImageManager::DebugBack, { 0.0f,0.0f });
	back.reset(back_);
	
	ActorManager::GetInstance()->AttachActor("Player");
	Chara[MPlayer] = ActorManager::GetInstance()->SearchActor("Player");
	ActorManager::GetInstance()->AttachActor("Bullet");
	Chara[MBullet] = ActorManager::GetInstance()->SearchActor("Bullet");
	ActorManager::GetInstance()->AttachActor("Enemy");
	Chara[MEnemy]=ActorManager::GetInstance()->SearchActor("Enemy");
	camera->SetEye({0, 0, -10});
}
//開放処理
void DebugScene::Finalize() {
	//３ｄのモデルのデリート
	ActorManager::GetInstance()->Finalize();
}
//更新
void DebugScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
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
	if (input->PushKey(DIK_0)) {
		int a = 0;
		a++;
	}
	Chara[Select]->Demo();
	if (input->TriggerButton(input->Select)) {
		SceneManager::GetInstance()->ChangeScene("PLAY");
	}
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
		ImGui::SetNextWindowPos(ImVec2(980, 300), 1);
		ImGui::SetNextWindowSize(ImVec2(280, 300), 1);
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
		ImGui::SetNextWindowPos(ImVec2(980, 300), 1);
		ImGui::SetNextWindowSize(ImVec2(280, 300), 1);
		ImGui::Begin("bulletParameter");
		//ImGui::SliderFloat("cameraPos.y", &cameraPos.y, 30, 0);
		ImGui::Unindent();
		ImGui::End();
	}
	if (Select == 2) {
		ImGui::SetNextWindowPos(ImVec2(980, 300), 1);
		ImGui::SetNextWindowSize(ImVec2(280, 300), 1);
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

	Sprite::PreDraw();
	back->Draw();
	Chara[Select]->DemoDraw();

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
	file << "//弾のストック" << std::endl;
	file << "STOCK" << ',';
	file << stock << ',';
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


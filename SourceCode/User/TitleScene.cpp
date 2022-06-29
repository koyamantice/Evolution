#include "TitleScene.h"
#include"SceneManager.h"
#include "AudioManager.h"
#include "input.h"
#include<string>
#include "DebugText.h"
#include"ImageManager.h"
#include"ModelManager.h"
#include <TisGame.h>
#include"Player.h"
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	Texture::SetCamera(camera);
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);
	// モデル読み込み
	//AudioManager::GetInstance()->LoadSound(1, "Resources/Sound/titleBGM.wav");
	//AudioManager::GetInstance()->LoopWave(1, 0.12f);
	//srand(NULL);
	// ライト生成
	lightGroup = LightGroup::Create();
	// 3Dオブエクトにライトをセット
	Object3d::SetLightGroup(lightGroup);

	//この順番で書かないと例外スローになります
	// デバイスをセット
	FBXObject3d::SetDevice(dxCommon->GetDev());
	// カメラをセット
	FBXObject3d::SetCamera(camera);
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();
	//背景スプライト生成
	//sprite[back] = Sprite::Create(ImageManager::TITLE, { 0.0f,0.0f });
	//スプライト生成
	Actor* Act_[Chr_Max]{};
	Act_[MPlayer] = new Player();
	Act_[MPlayer]->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Player));
	Act_[Enemy] = new Actor();
	Act_[Enemy]->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Player));
	Act_[Enemy]->SetPosition({1,0,0});
	actor[MPlayer].reset(Act_[MPlayer]);
	actor[Enemy].reset(Act_[Enemy]);
}
//開放処理
void TitleScene::Finalize() {
	//３ｄのモデルのデリート
	for (int i = 0; i < 4;i++) {
		delete sprite[i];
	}
}
//更新
void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	camera->Update();
	actor[MPlayer]->Update();
	actor[Enemy]->Update();


	for (int i = 0; i < Chr_Max;i++) {
		for (int j = 1; j < Chr_Max; j++) {
			if (i == j) { return; }
			if (!actor[i]->Collide(actor[j].get())) { return; }
			int a = 0;
			a++;
		}
	}


}
//描画
void TitleScene::Draw(DirectXCommon* dxCommon) {
	ImGui::Begin("test");
	//ImGui::SliderFloat("cameraPos.y", &cameraPos.y, 30, 0);
	ImGui::Text("P::%f", buttonPos[1].x);
	ImGui::Unindent();
	ImGui::End();
	Sprite::PreDraw();
	//背景用
	actor[MPlayer]->Draw();
	actor[Enemy]->Draw();

}


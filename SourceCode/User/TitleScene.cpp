#include "TitleScene.h"
#include"SceneManager.h"
#include "Audio.h"
#include "input.h"
#include<string>
#include "DebugText.h"
#include"ImageManager.h"
#include"ModelManager.h"
#include <TisGame.h>
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	Texture::SetCamera(camera);
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);
	// モデル読み込み
	Audio::GetInstance()->LoadSound(1, "Resources/Sound/titleBGM.wav");
	Audio::GetInstance()->LoopWave(1, 0.12f);
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
	sprite[back] = Sprite::Create(ImageManager::TITLE, { 0.0f,0.0f });
	sprite[button] = Sprite::Create(ImageManager::button, buttonPos[0]);
	sprite[button]->SetAnchorPoint({ 0.5f,0.5f });
	//sprite[button]->SetScale(0.4f);
	sprite[select] = Sprite::Create(ImageManager::Tselect, { 76.0f,418.0f });
	//スプライト生成
	Actor* Act_[2]{};
	Act_[0] = new Actor(std::string("player"));
	Act_[0]->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Player));


	actor[Player].reset(Act_[Player]);
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
	actor[Player]->Update();
}
//描画
void TitleScene::Draw(DirectXCommon* dxCommon) {
	//ImGui::Begin("test");
	////ImGui::SliderFloat("cameraPos.y", &cameraPos.y, 30, 0);
	//ImGui::Text("P::%d", P);
	//ImGui::Unindent();
	//ImGui::End();
	Sprite::PreDraw();
	//背景用
	sprite[back]->Draw();
	sprite[select]->Draw();
	sprite[select]->Draw();
	sprite[button]->Draw();

	actor[Player]->Draw();

}


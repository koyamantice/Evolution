#include "TitleScene.h"
#include"SceneManager.h"
#include "AudioManager.h"
#include "input.h"
#include<string>
#include"ImageManager.h"
#include"ModelManager.h"
#include <TisGame.h>
#include"ActorManager.h"
#include"Easing.h"

static int LoadState=0;
std::thread t;


void TitleScene::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	camera->SetEye({0,0,-20});

	//背景スプライト生成
	Sprite* sprite_ = Sprite::Create(ImageManager::Title, { 0.0f,0.0f });
	UI[BackScene].reset(sprite_);
	Sprite* sprite2_ = Sprite::Create(ImageManager::Button, { 880.0f,600.0f });
	sprite2_->SetScale(0.5f);
	UI[Button].reset(sprite2_);

	Sprite* sprite3_ = Sprite::Create(ImageManager::TitleMove, { 880.0f,360.0f });
	sprite3_->SetScale(0.5f);
	UI[moveTitle].reset(sprite3_);

	Sprite* sprite6_ = Sprite::Create(ImageManager::noTitleMove, { 880.0f,360.0f });
	sprite6_->SetScale(0.5f);
	UI[nomoveTitle].reset(sprite6_);

	Sprite* sprite4_ = Sprite::Create(ImageManager::DebugMove, { 880.0f,500.0f });
	sprite4_->SetScale(0.5f);
	sprite4_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	UI[moveDebug].reset(sprite4_);
	Change = false;

	Sprite* sprite5_ = Sprite::Create(ImageManager::noDebugMove, { 880.0f,500.0f });
	sprite5_->SetScale(0.5f);
	sprite5_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	UI[nomoveDebug].reset(sprite5_);
	Change = false;

	text = new TitleText();
	text->Init();
}
//開放処理
void TitleScene::Finalize() {
	//３ｄのモデルのデリート
}
//更新
void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();

	camera->Update();
	if (input->TiltStick(Input::L_UP) ||input->TriggerButton(Input::UP) || input->TriggerKey(DIK_UP)) {
		nextScene--;
	}
	if (input->TiltStick(Input::L_DOWN) || input->TriggerButton(Input::DOWN) || input->TriggerKey(DIK_DOWN)) {
		nextScene++;
	}
	if (nextScene < Portal::Title) {
		nextScene = Portal::Title;
	}
	if (nextScene > Portal::Debug) {
		nextScene = Portal::Debug;
	}
	if (nextScene == Portal::Title) {
		UI[moveTitle]->SetColor({ 1.0f,1.0f,1.0f,0.0f });
		UI[moveTitle]->SetSize({ 567.8f/2.0f,192.6f/2.0f });
		UI[nomoveTitle]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		UI[nomoveTitle]->SetSize({ 567.8f / 2.0f,192.6f / 2.0f });

		UI[moveDebug]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		UI[moveDebug]->SetSize({ 567.8f/2,192.6f/2});
		UI[nomoveDebug]->SetColor({ 1.0f,1.0f,1.0f,0.0f });
		UI[nomoveDebug]->SetSize({ 567.8f / 2,192.6f / 2 });

		moveTitleNow = 1;
		moveDebugNow = 2;
	}
	if (nextScene == Portal::Debug) {

		UI[moveTitle]->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
		UI[moveTitle]->SetSize({ 567.8f / 2.0f,192.6f / 2.0f });
		UI[nomoveTitle]->SetColor({ 1.0f,1.0f,1.0f,0.0f });
		UI[nomoveTitle]->SetSize({ 567.8f / 2.0f,192.6f / 2.0f });

		UI[moveDebug]->SetColor({ 1.0f,1.0f,1.0f,0.0f });
		UI[moveDebug]->SetSize({ 567.8f / 2,192.6f / 2 });
		UI[nomoveDebug]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		UI[nomoveDebug]->SetSize({ 567.8f / 2,192.6f / 2 });

		moveTitleNow = 0;
		moveDebugNow = 1;
	}




	if (input->TriggerKey(DIK_SPACE)||input->TriggerButton(input->A)) {
		Change = true;
		switch (nextScene) {
		case Portal::Title:
			SceneName = "PLAY";
			break;
		case Portal::Debug:
			SceneName = "MAP";
			break;
		default:
			assert(0);
			break;
		}
	}
	Feed(SceneName);
	text->Upda();
}
//描画
void TitleScene::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	//ImGui::Begin("test");
	//float F = FPSManager::GetInstance()->GetFps();
	//ImGui::SliderFloat("fps", &F, 120, 0);
	//ImGui::SliderFloat("cameraPos.y", &A, 35000, 0);
	//ImGui::Unindent();
	//ImGui::End();
	Sprite::PreDraw();
	Object3d::PreDraw();
	text->Draw(dxCommon);
	Sprite::PreDraw();
	for (int i = 1; i < SpriteMax; i++) {
		UI[i]->Draw();
	}
	UI[0]->Draw();

	Effect->Draw();

	dxCommon->PostDraw();
}

void TitleScene::Heavy() {
}

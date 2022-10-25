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
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	//背景スプライト生成
	Sprite* sprite_ = Sprite::Create(ImageManager::Title, { 0.0f,0.0f });
	UI[BackScene].reset(sprite_);
	Sprite* sprite2_ = Sprite::Create(ImageManager::Button, { 640.0f,650.0f });
	sprite2_->SetScale(0.5f);
	UI[Button].reset(sprite2_);
	Sprite* sprite3_ = Sprite::Create(ImageManager::TitleMove, { 640.0f,360.0f });
	sprite3_->SetScale(0.7f);
	UI[moveTitle].reset(sprite3_);
	Sprite* sprite4_ = Sprite::Create(ImageManager::DebugMove, { 640.0f,460.0f });
	sprite4_->SetScale(0.5f);
	sprite4_->SetColor({1.0f,1.0f,1.0f,0.5f});
	UI[moveDebug].reset(sprite4_);


	//スプライト生成
	Sprite* Effect_ = Sprite::Create(ImageManager::Black, { 0.0f,0.0f });
	Effect.reset(Effect_);
	Effect->SetColor({1,1,1,alpha});

	TitleText* text_ = new TitleText();
	text_->Init();
	text.reset(text_);
}
//開放処理
void TitleScene::Finalize() {
	//３ｄのモデルのデリート
	ActorManager::GetInstance()->Finalize();
}
//更新
void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	camera->Update();
	ActorManager::GetInstance()->Update();

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
		UI[moveTitle]->SetColor({ 0.34f,0.56f,0.96f,1.0f });
		UI[moveTitle]->SetSize({ 268.8f,89.6f });

		UI[moveDebug]->SetColor({ 1.0f,1.0f,1.0f,0.5f });
		UI[moveDebug]->SetSize({192.0f, 64.0f});

		moveTitleNow = 1;
		moveDebugNow = 2;
	}
	if (nextScene == Portal::Debug) {
		UI[moveTitle]->SetColor({1.0f, 1.0f, 1.0f, 0.5f});
		UI[moveTitle]->SetSize({ 192.0f, 64.0f });

		UI[moveDebug]->SetColor({ 0.34f,0.56f,0.96f,1.0f });
		UI[moveDebug]->SetSize({ 268.8f,89.6f });

		moveTitleNow = 0;
		moveDebugNow = 1;
	}




	moveText[0] = Ease(In,Quad, 0.7f,UI[moveTitle]->GetPosition().y,SetPos[moveTitleNow].y);
	moveText[1] = Ease(In, Quad,0.7f,UI[moveDebug]->GetPosition().y,SetPos[moveDebugNow].y);
	UI[moveTitle]->SetPosition({640,moveText[0]});
	UI[moveDebug]->SetPosition({640,moveText[1]});




	if (input->TriggerKey(DIK_SPACE)||input->TriggerButton(input->A)) {
		Change = true;
		switch (nextScene) {
		case Portal::Title:
			SceneName = "PLAY";
			break;
		case Portal::Debug:
			SceneName = "DEBUG";
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
	ImGui::Begin("test");
	float F = FPSManager::GetInstance()->GetFps();
	ImGui::SliderFloat("fps", &F, 120, 0);
	//ImGui::SliderFloat("cameraPos.y", &A, 35000, 0);
	//ImGui::Unindent();
	ImGui::End();
	Sprite::PreDraw();
	for (int i = 0; i < SpriteMax;i++) {
		UI[i]->Draw();
	}
	Object3d::PreDraw();
	text->Draw(dxCommon);
	ActorManager::GetInstance()->Draw(dxCommon);
	Sprite::PreDraw();
	Effect->Draw();
	dxCommon->PostDraw();

}

void TitleScene::Feed(const std::string& sceneName) {
	if (Change) {
		if (frame < 1.6f) {
			frame += 0.02f;
		} else {
			SceneManager::GetInstance()->ChangeScene(sceneName);
		}
		alpha = Ease(In, Cubic, frame, 0, 1);
		Effect->SetColor({ 1,1,1,alpha });
	}

}


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


	c_x = sinf(rad * (XM_PI / 180.f)) * circ_x;
	c_z = cosf(rad * (XM_PI / 180.f)) * circ_z;
	camera->SetEye({ c_x,c_y,c_z });

	//背景スプライト生成
	Sprite* sprite_ = Sprite::Create(ImageManager::Title, { 0.0f,0.0f });
	UI[BackScene].reset(sprite_);

	Sprite* sprite1_ = Sprite::Create(ImageManager::DebugBack, { 0.0f,0.0f });
	sprite1_->SetSize({1280.0f, 720.0f});
	UI[kkskydome].reset(sprite1_);

	
	Sprite* sprite2_ = Sprite::Create(ImageManager::Button, { 950.0f,650.0f });
	sprite2_->SetScale(0.5f);
	UI[Button].reset(sprite2_);

	Sprite* sprite3_ = Sprite::Create(ImageManager::TitleMove, { 930.0f,360.0f });
	sprite3_->SetScale(0.5f);
	UI[moveTitle].reset(sprite3_);

	Sprite* sprite6_ = Sprite::Create(ImageManager::noTitleMove, { 930.0f,360.0f });
	sprite6_->SetScale(0.5f);
	UI[nomoveTitle].reset(sprite6_);

	Sprite* sprite4_ = Sprite::Create(ImageManager::DebugMove, { 930.0f,500.0f });
	sprite4_->SetScale(0.5f);
	sprite4_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	UI[moveDebug].reset(sprite4_);

	Sprite* sprite5_ = Sprite::Create(ImageManager::noDebugMove, { 930.0f,500.0f });
	sprite5_->SetScale(0.5f);
	sprite5_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	UI[nomoveDebug].reset(sprite5_);

	text = new TitleText();
	text->Init();


	//あとでcsv
	eyes.push_back(80);
	eyes.push_back(40);
	eyes.push_back(50);

}
//開放処理
void TitleScene::Finalize() {
	//３ｄのモデルのデリート
}
//更新
void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	CameraUpdate();
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
		scene_changer->ChangeStart();
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
	scene_changer->ChangeScene(SceneName);
	text->Upda();
}
//描画
void TitleScene::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	//ImGui::Begin("test");
	//float F = FPSManager::GetInstance()->GetFps();
	//ImGui::SliderFloat("fps", &F, 120, 0);
	//ImGui::SliderFloat("cameraPos.x", &c_x, 1000, 0);
	//ImGui::SliderFloat("cameraPos.y", &c_y, 1000, 0);
	//ImGui::SliderFloat("cameraPos.z", &c_z, 1000, 0);
	//float t=eyes.front();
	//ImGui::SliderFloat("eye", &t, 1000, 0);
	//ImGui::Unindent();
	//ImGui::End();
	Sprite::PreDraw();
	UI[1]->Draw();
	Object3d::PreDraw();
	text->Draw(dxCommon);
	Sprite::PreDraw();
	for (int i = 2; i < SpriteMax; i++) {
		UI[i]->Draw();
	}
//	UI[0]->Draw();

	scene_changer->Draw();

	dxCommon->PostDraw();
}

void TitleScene::Heavy() {
}

void TitleScene::CameraUpdate() {
	rad_frame += 0.0008f;
	
	rad= Ease(In, Linear, rad_frame,0, 360);
	circ_x = Ease(In, Linear, rad_frame, s_camera.x, e_camera.x);
	circ_z = Ease(In, Linear, rad_frame, s_camera.z, e_camera.z);
	if (rad_frame > 1.0f) {
		rad_frame = 0;
		s_camera.x=e_camera.x;
		s_camera.z=e_camera.z;
		auto s = eyes.front();
		e_camera.x= s;
		e_camera.z = s;
		eyes.pop_front();
		eyes.push_back(s);
	}
	c_x=sinf(rad* (XM_PI/180.f)) * circ_x;
	c_z=cosf(rad* (XM_PI/180.f)) * circ_z;

	camera->SetEye({c_x,c_y,c_z});
	camera->Update();

}

#include "TitleScene.h"


static int LoadState=0;
std::thread t;

void TitleScene::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);


	c_x = sinf(rad * (XM_PI / DEGREE_HALF)) * circ_x;
	c_z = cosf(rad * (XM_PI / DEGREE_HALF)) * circ_z;
	camera->SetEye({ c_x,c_y,c_z });


	for (int i = 0; i < SpriteMax;i++) {
		Sprite* sprite_ = Sprite::Create(ImageManager::Title + i, { 0.0f,0.0f });
		if (i != BackScene) {
			sprite_->SetScale(0.5f);
			if (i <= nomoveDebug) {
				sprite_->SetPosition(kWord2DebugPos);
			} else {
				sprite_->SetPosition(kWord2TitlegPos);
			}
		}
		UI[i].reset(sprite_);
	}

	text = std::make_unique<TitleText>();

	audioManager = std::make_unique<AudioManager>();
	
	audioManager->LoadWave("BGM/titleBGM.wav");
	audioManager->LoadWave("SE/pushstart.wav");

	audioManager->PlayWave("BGM/titleBGM.wav",0.5f);


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
		nextScene = Portal::Debug;
	}
	if (nextScene > Portal::Debug) {
		nextScene = Portal::Title;
	}
	if (nextScene == Portal::Title) {
		UI[moveTitle]->SetColor(kInVisivleColor);
		UI[nomoveTitle]->SetColor(kVisivleColor);

		UI[moveDebug]->SetColor(kVisivleColor);
		UI[nomoveDebug]->SetColor(kInVisivleColor);
	}
	if (nextScene == Portal::Debug) {

		UI[moveTitle]->SetColor(kVisivleColor);
		UI[nomoveTitle]->SetColor(kInVisivleColor);

		UI[moveDebug]->SetColor(kInVisivleColor);
		UI[nomoveDebug]->SetColor(kVisivleColor);
	}

	if (input->TriggerKey(DIK_SPACE)||
		input->TriggerButton(input->A)||
		input->TriggerButton(input->B)||
		input->TriggerButton(input->X)||
		input->TriggerButton(input->Y)) {
		scene_changer->ChangeStart();
		audioManager->PlayWave("SE/pushstart.wav",0.5f);
		switch (nextScene) {
		case Portal::Title:
			SceneName = "FIRSTSTAGE";
			break;
		case Portal::Debug:
			SceneName = "MSECOND";
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

	Sprite::PreDraw();
	UI[BackScene]->Draw();
	Object3d::PreDraw();
	text->Draw(dxCommon);
	Sprite::PreDraw();
	for (int i = moveDebug; i < SpriteMax; i++) {
		UI[i]->Draw();
	}
	scene_changer->Draw();

	dxCommon->PostDraw();
}

void TitleScene::Heavy() {
}

void TitleScene::CameraUpdate() {
	rad_frame += 1.0f/kRoundFrameMax;
	
	rad= Ease(In, Linear, rad_frame,0, DEGREE_MAX);
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
	c_x=sinf(rad* (XM_PI/ DEGREE_HALF)) * circ_x;
	c_z=cosf(rad* (XM_PI/ DEGREE_HALF)) * circ_z;

	camera->SetEye({c_x,c_y,c_z});
	camera->Update();

}

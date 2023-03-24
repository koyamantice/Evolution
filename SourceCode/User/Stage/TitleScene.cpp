#include "TitleScene.h"


static int LoadState = 0;
std::thread t;

void TitleScene::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);

	BossLevelLoader& gFoo = Singleton<BossLevelLoader>::get_instance();
	gFoo.LoadData("BossLevel");
	
	lightGroup->SetDirLightActive(0,true);
	lightGroup->SetDirLightActive(1,true);
	lightGroup->SetDirLightActive(2,true);
	lightGroup->DefaultLightSetting();
	lightGroup->SetPointLightActive(0,true);
	pointLightPos[0]=0.5f;
	pointLightPos[1]=5.0f;
	pointLightPos[2]=0.0f;

	c_x = sinf(rad * (XM_PI / DEGREE_HALF)) * circ_x;
	c_z = cosf(rad * (XM_PI / DEGREE_HALF)) * circ_z;
	camera->SetEye({ c_x,c_y,c_z });


	for (int i = 0; i < SpriteMax; i++) {
		UI[i] = Sprite::Create(ImageManager::Title + i, { 0.0f,0.0f });
		if (i != BackScene) {
			UI[i]->SetScale(0.5f);
			if (i <= nomoveDebug) {
				UI[i]->SetPosition(kWord2DebugPos);
			} else {
				UI[i]->SetPosition(kWord2TitlegPos);
			}
		}
	}

	text = std::make_unique<TitleText>();

	audioManager = std::make_unique<AudioManager>();

	audioManager->LoadWave("BGM/titleBGM.wav");
	audioManager->LoadWave("SE/pushstart.wav");

	audioManager->PlayWave("BGM/titleBGM.wav", 0.3f);


	//あとでcsv
	eyes.push_back(70);
	eyes.push_back(60);
	eyes.push_back(50);
	auto s = eyes.front();
	e_camera = { s,0,s };

}
//開放処理
void TitleScene::Finalize() {
	//３ｄのモデルのデリート
}
//更新
void TitleScene::Update(DirectXCommon* dxCommon) {
	CameraUpdate();
	lightGroup->SetPointLightPos(0,XMFLOAT3(pointLightPos));
	lightGroup->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	lightGroup->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));
	
	
	text->Upda();

	if (sceneChanger_->GetEasingStart()) { sceneChanger_->ChangeScene(SceneName); return; }
	InputUpdate();
	SelectTextUpdate();
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
	sceneChanger_->Draw();

	dxCommon->PostDraw();
}

void TitleScene::SelectTextUpdate() {
	//ゆっくり浮かび上がっていく処理
	sin_frame_ += sin_add_;

	if (nextScene == Portal::Title) {
		UI[moveTitle]->SetColor(kInVisivleColor);
		UI[nomoveTitle]->SetColor(kVisivleColor);

		UI[nomoveTitle]->SetPosition({ kWord2TitlegPos.x,kWord2TitlegPos.y + sinf(sin_frame_) * 10.0f });

		UI[moveDebug]->SetColor(kVisivleColor);
		UI[nomoveDebug]->SetColor(kInVisivleColor);
	}
	if (nextScene == Portal::Debug) {

		UI[moveTitle]->SetColor(kVisivleColor);
		UI[nomoveTitle]->SetColor(kInVisivleColor);

		UI[moveDebug]->SetColor(kInVisivleColor);
		UI[nomoveDebug]->SetColor(kVisivleColor);

		UI[nomoveDebug]->SetPosition({ kWord2DebugPos.x,kWord2DebugPos.y + sinf(sin_frame_) * 10.0f });
	}

}

void TitleScene::InputUpdate() {
	Input* input = Input::GetInstance();

	if (input->TiltStick(Input::L_UP) || input->TriggerButton(Input::UP) || input->TriggerKey(DIK_UP)) {
		nextScene--;
		sin_frame_ = 0.0f;
	}
	if (input->TiltStick(Input::L_DOWN) || input->TriggerButton(Input::DOWN) || input->TriggerKey(DIK_DOWN)) {
		nextScene++;
		sin_frame_ = 0.0f;
	}
	if (nextScene < Portal::Title) { nextScene = Portal::Debug; }
	if (nextScene > Portal::Debug) { nextScene = Portal::Title; }

	if (input->TriggerKey(DIK_SPACE) ||
		input->TriggerButton(input->A) ||
		input->TriggerButton(input->B) ||
		input->TriggerButton(input->X) ||
		input->TriggerButton(input->Y)) {
		sceneChanger_->ChangeStart();
		audioManager->StopWave("BGM/titleBGM.wav");
		audioManager->PlayWave("SE/pushstart.wav", 0.3f);
		switch (nextScene) {
		case Portal::Title:
			SceneName = "FIRSTSTAGE";
			break;
		case Portal::Debug:
			SceneName = "SECONDSTAGE";
			break;
		default:
			assert(0);
			break;
		}
	}


}

void TitleScene::CameraUpdate() {
	rad_frame += 1.0f / kRoundFrameMax;

	rad = Ease(In, Linear, rad_frame, 0, DEGREE_MAX);
	circ_x = Ease(In, Linear, rad_frame, s_camera.x, e_camera.x);
	circ_z = Ease(In, Linear, rad_frame, s_camera.z, e_camera.z);
	if (rad_frame > 1.0f) {
		rad_frame = 0;
		s_camera.x = e_camera.x;
		s_camera.z = e_camera.z;
		auto s = eyes.front();
		e_camera.x = s;
		e_camera.z = s;
		eyes.pop_front();
		eyes.push_back(s);
	}
	c_x = sinf(rad * (XM_PI / DEGREE_HALF)) * circ_x;
	c_z = cosf(rad * (XM_PI / DEGREE_HALF)) * circ_z;

	camera->SetEye({ c_x,c_y,c_z });
	camera->Update();

}

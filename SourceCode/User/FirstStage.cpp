#include "FirstStage.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "input.h"
#include "ModelManager.h"
#include <TisGame.h>
#include "Player.h"
#include "PlayerUI.h"
#include"Enemy.h"
#include <SourceCode/Common/Easing.h>
#include"ActorManager.h"

void FirstStage::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);

	BattleInit();
	TorchSetup(Touch::FireColor::f_green);

	//ゲームアクターの生成をします。
	ActorManager::GetInstance()->AttachActor("Player");
	ActorManager::GetInstance()->AttachActor("Enemy");
	ActorManager::GetInstance()->AttachActor("ClearCrystal");
	for (int i = 0; i < 10; i++) {
		ActorManager::GetInstance()->AttachBullet("Red");
	}
	for (int i = 0; i < 10; i++) {
		ActorManager::GetInstance()->AttachBullet("Green");
	}
	//シーン内で必要なアクターを参照します。
	player_shadow = ActorManager::GetInstance()->SearchActor("Player");
	enemy_shadow = ActorManager::GetInstance()->SearchActor("Enemy");
	goal_shadow = ActorManager::GetInstance()->SearchActor("ClearCrystal");
	goal_shadow->SetPosition(enemy_shadow->GetPosition());
	goal_shadow->SetIsActive(false);


	//カメラの操作説明
	const int w = 256;
	const int h = 256;
	const int l = 6;
	for (int i = 0; i < 6; i++) {
		Sprite* CameCon_[6]{};
		CameCon_[i] = Sprite::Create(ImageManager::CameCon, { 0,0 });
		Camecon[i].reset(CameCon_[i]);
		int number_index_y = i / l;
		int number_index_x = i % l;
		Camecon[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * w, static_cast<float>(number_index_y) * h },
			{ static_cast<float>(w), static_cast<float>(h) });
		Camecon[i]->SetSize({ 128,128 });
		Camecon[i]->SetScale(1.0f);
		Camecon[i]->SetAnchorPoint({ 0.5f,0.5f });
	}
	Camecon[0]->SetPosition({ base.x + Camecon[0]->GetSize().x,base.y });
	Camecon[1]->SetPosition({ base.x + Camecon[1]->GetSize().x,base.y });
	Camecon[2]->SetPosition({ base.x + Camecon[2]->GetSize().x,base.y });
	Camecon[3]->SetPosition({ base.x - Camecon[3]->GetSize().x,base.y });
	Camecon[4]->SetPosition({ base.x - Camecon[4]->GetSize().x,base.y });
	Camecon[5]->SetPosition(base);
	
	//コントローラー説明の表示
	const int w_2 = 512;
	const int p = 2;
	for (int i = 0; i < 2; i++) {
		Sprite* con_vis_[2]{};
		con_vis_[i] = Sprite::Create(ImageManager::Con_vis, { 200,360 });
		con_vis[i].reset(con_vis_[i]);
		int number_index_y = i / p;
		int number_index_x = i % p;
		con_vis[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * w_2, static_cast<float>(number_index_y) * h },
			{ static_cast<float>(w_2), static_cast<float>(h) });
		con_vis[i]->SetSize({ 128,64 });
		con_vis[i]->SetAnchorPoint({ 0.5f,0.5f });
	}
	con_vis[0]->SetPosition({70,520});
	con_vis[1]->SetPosition({70,600});

	//スプライト生成
	Sprite* _clear = nullptr;
	_clear = Sprite::Create(ImageManager::Clear, { 0,0 });
	Clear.reset(_clear);

	Sprite* _Over = nullptr;
	_Over = Sprite::Create(ImageManager::Over, { 0,0 });
	Over.reset(_Over);


	//導入部分の言葉
	for(int i=0;i<intro_word_max;i++){ 
		Sprite* IntroWord_ = Sprite::Create(ImageManager::Intro01 + i, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
		IntroWord[i].reset(IntroWord_);	
	}

	camera->SetTarget(player_shadow->GetPosition());
	distance.x = sinf(angle * (XM_PI / 180)) * 13.0f;
	distance.y = cosf(angle * (XM_PI / 180)) * 13.0f;
	dis.x = distance.x;
	dis.y = distance.y;


	postEffect = new PostEffect();
	postEffect->Initialize();

	partMan = new ParticleManager();
	partMan->Initialize(ImageManager::charge);
}
//開放処理
void FirstStage::Finalize() {
	//３ｄのモデルのデリート
	ActorManager::GetInstance()->Finalize();
}
//更新
void FirstStage::Update(DirectXCommon* dxCommon) {
#pragma region "Clear"
	if (clear) {
		ClearUpdate();
		return;
	}
	if (enemy_shadow->GetPause()) {
		const float rnd_vel = 0.4f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		partMan->Add(120, enemy_shadow->GetPosition(), vel, XMFLOAT3(), 1.2f, 0.0f, { 1,1,1,1 }, { 1,1,1,0 });
		partMan->Update();

		finishTime++;

		if (finishTime > 200) {
			enemy_shadow->SetPause(false);
			enemy_shadow->SetCommand(Actor::DEAD);
		}
		return;
	}
	if (!enemy_shadow->GetIsActive()) {
		goal_shadow->SetIsActive(true);
		if (goal_shadow->GetPause() || player_shadow->GetPause()) {
			Result = true;
			clear = true;
		}
	}
#pragma endregion
#pragma region "GameOver"
	if (ActorManager::GetInstance()->SearchNum("Bullet") <= 0) {
		GameOver = true;
		if (input->TriggerButton(Input::A)) {
			SceneManager::GetInstance()->ChangeScene("TITLE");
		}
	}
#pragma endregion
	if (Intro) {
		if (scene_first_change) {
			if (introFrame < 1.0f) {
				introFrame += 0.01f;
			} else {
				scene_first_change = false;
			}
			filter_alpha = Ease(In, Cubic, introFrame, 1, 0);
			filter_first->SetColor({ 1,1,1,filter_alpha });
		}
		IntroCamera(count);
		ActorManager::GetInstance()->IntroUpdate(count);
		if (count > 1200) {
			filter_first->SetColor({ 1,1,1,0 });
			count = 0;
			Intro = false;
		}
		count += speed;
		if (input->PushButton(input->START)) {
			if (count % 2 == 1) {
				count--;
			}
			speed = 2;
		}
		if (count % 200 == 0) {
			if (nowWord != 5) {
				nowWord++;
			}
		}
		FieldUpdate();
		return;
	}
	if (pause) {
		pauseUi->Update();
		if (pauseUi->GetEndFlag()) {
			pause = false;
		}
		return;
	}

	if (input->TriggerButton(input->START)) {
		pause = true;
		if (pauseUi->GetEndFlag()) {
			pauseUi->SetEndFlag(false);
		}
	}

	DescriptionUpdate();

	ActorManager::GetInstance()->Update();

	CameraUpda();

	FieldUpdate();

	partMan->Update();
}

void FirstStage::CameraUpda() {
	if (Reset) {
		ResetCamera();
		return;
	}
	if (input->TiltPushStick(Input::R_RIGHT) || input->TiltPushStick(Input::R_LEFT)) {
		if (!cameraExplanation) {
			cameraExplanation = true;
		}
		if (!pauseUi->GetReverseCamera()) {
			if (input->TiltPushStick(Input::R_RIGHT)) {
				angle -= 3;
			}
			if (input->TiltPushStick(Input::R_LEFT)) {
				angle += 3;
			}
		} else {
			if (input->TiltPushStick(Input::R_RIGHT)) {
				angle += 3;
			}
			if (input->TiltPushStick(Input::R_LEFT)) {
				angle -= 3;
			}
		}
		if (angle > 360 || angle < 0) {
			angle += 360;
			angle = (float)((int)angle % 360);
		}
		dis.x = sinf(angle * (XM_PI / 180)) * 15.0f;
		dis.y = cosf(angle * (XM_PI / 180)) * 15.0f;
		distance.x = Ease(In, Quad, 0.6f, distance.x, dis.x);
		distance.y = Ease(In, Quad, 0.6f, distance.y, dis.y);

	}
	if (input->TriggerButton(Input::LT)) {
		if (!Reset) {
			if (!cameraExplanation) {
				cameraExplanation = true;
			}
			angleframe = 0.0f;
			firstangle = angle;
			endangle = player_shadow->GetRotation().y;
			if (abs(endangle-firstangle)>=180) {
				if (endangle > firstangle) {
					firstangle -= 360;
				} else {
					endangle += 360;
				}
			}

			firstdis = distance;
			Reset = true;
		}
	}
	player_shadow->SetAngle(angle);
	camera->SetTarget(player_shadow->GetCameraPos(angle, 7));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + hight,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}

void FirstStage::IntroCamera(int Timer) {
	if (Timer <= 720) {
		if (speed == 1) {
			angle += 0.5f;
			if (IntroHight > hight) {
				IntroHight -= 0.075f;
			} else {
				IntroHight = hight;
			}
		} else {
			angle += 1.0f;
			if (IntroHight > hight) {
				IntroHight -= 0.150f;
			} else {
				IntroHight = hight;
			}
		}
	}

	dis.x = sinf(angle * (XM_PI / 180)) * 13.0f;
	dis.y = cosf(angle * (XM_PI / 180)) * 13.0f;
	distance.x = Ease(In, Quad, 0.6f, distance.x, dis.x);
	distance.y = Ease(In, Quad, 0.6f, distance.y, dis.y);
	player_shadow->SetAngle(angle);
	camera->SetTarget(player_shadow->GetCameraPos(angle, 7));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,IntroHight,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}

void FirstStage::ResultCamera(int Timer) {
	if (Timer <= 720) {
		angle += 0.5f;
	}
	//player_shadow->SetAngle(angle);
	camera->SetTarget(goal_shadow->GetPosition());
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + hight,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}

//描画
void FirstStage::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	//postEffect->PreDrawScene(dxCommon->GetCmdList());
	ImGui::Begin("playscene");
	ImGui::SliderFloat("firstangle", &firstangle, 0, 360);
	ImGui::SliderFloat("endangle", &endangle, 0, 360);
	ImGui::SliderFloat("Anglet", &angle, 0, 360);
	float y= player_shadow->GetRotation().y;
	ImGui::SliderFloat("rot", &y, 0, 360);

	ImGui::End();
	Object3d::PreDraw();
	skydome->Draw();
	ground->Draw();
	for (std::unique_ptr<Touch>& touch : torchs) {
		touch->Draw();
	}
	//背景用
	ActorManager::GetInstance()->Draw(dxCommon);
	partMan->Draw(alphaBle);
	Sprite::PreDraw();
	if (scene_first_change) {
		filter_first->Draw();
	}
	if (clear) {
		Clear->Draw();
	}
	if (Intro) {
		screens[0]->Draw();
		screens[1]->Draw();
		IntroWord[nowWord]->Draw();
	} else {
		con_vis[0]->Draw();
		con_vis[1]->Draw();
		if (!clear) {
			Camecon[animation]->Draw();
			Camecon[tapanima]->Draw();
			Camecon[5]->Draw();
		}
	}
	if (Result) {
		screens[0]->Draw();
		screens[1]->Draw();
		Clear->Draw();
	}
	if (GameOver) {
		Over->Draw();
	}
	if (pause) {
		pauseUi->Draw();
	}
	dxCommon->PostDraw();
}

void FirstStage::ResetCamera() {
	player_shadow->SetCanMove(false);

	if (angleframe < 1.0f) {
		angleframe += 0.05f;
	} else {
		Reset = false;
		player_shadow->SetCanMove(true);
		angleframe = 1.0f;
		distance.x = dis.x;
		distance.x = dis.y;
	}

	angle = Ease(In, Quad, angleframe, firstangle, endangle);

	dis.x = sinf(angle * (XM_PI / 180)) * 13.0f;
	dis.y = cosf(angle * (XM_PI / 180)) * 13.0f;

	distance.x = Ease(In, Quad, angleframe, firstdis.x, dis.x);
	distance.y = Ease(In, Quad, angleframe, firstdis.y, dis.y);
	camera->SetTarget(player_shadow->GetCameraPos(angle, 7));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + hight,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}

void FirstStage::DescriptionUpdate() {
	animafrate++;
	if (animafrate == 30) {
		if (animation < 2 && animation > 0) {
			animation += vec;
		} else if (animation == 2) {
			animation = 1;
			vec *= -1;
		} else if (animation == 0) {
			animation = 1;
			vec *= -1;
		}
		if (tapanima == 3) {
			tapanima = 4;
		} else {
			tapanima = 3;
		}
		animafrate = 0;
	}

	if (cameraExplanation) {
		if (camera_frame < 1.0f) {
			camera_frame += 0.005f;
		} else {
			camera_frame = 1.0f;
		}
		XMFLOAT2 siz{};
		for (int i = 0; i < 6; i++) {
			siz.x = Ease(In, Quad, camera_frame, 128, 64);
			siz.y = Ease(In, Quad, camera_frame, 128, 64);
			Camecon[i]->SetSize(siz);
		}
		base.x = Ease(In, Quad, camera_frame, 640, 90);
		base.y = Ease(In, Quad, camera_frame, 530, 680);
		Camecon[0]->SetPosition({ base.x + siz.x,base.y });
		Camecon[1]->SetPosition({ base.x + siz.x,base.y });
		Camecon[2]->SetPosition({ base.x + siz.x,base.y });
		Camecon[3]->SetPosition({ base.x - siz.x,base.y });
		Camecon[4]->SetPosition({ base.x - siz.x,base.y });
		Camecon[5]->SetPosition({ base.x       ,base.y });

	}
}

void FirstStage::ClearUpdate() {
	ResultCamera(count);
	count++;
	ActorManager::GetInstance()->ResultUpdate(count);
	const float rnd_vel = 0.5f;
	XMFLOAT3 vel{};
	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	partMan->Add(100, goal_shadow->GetPosition(), vel, XMFLOAT3(), 1.2f, 0.0f, { 1,1,0.5f,1 }, { 1,1,1,0.3f });
	partMan->Update();
	if (input->TriggerButton(Input::A) || input->TriggerButton(Input::B)) {
		scene_changer->ChangeStart();
	}
	scene_changer->ChangeScene("MAP");
	FieldUpdate();

}


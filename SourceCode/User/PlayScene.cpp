#include "PlayScene.h"
#include"SceneManager.h"
#include "AudioManager.h"
#include "input.h"
#include<string>
#include"ImageManager.h"
#include"ModelManager.h"
#include <TisGame.h>
#include"Player.h"
#include"PlayerUI.h"
#include"Enemy.h"
#include <SourceCode/Common/Easing.h>
#include"ActorManager.h"

void PlayScene::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	//�w�i�X�v���C�g����
	//sprite[back] = Sprite::Create(ImageManager::TITLE, { 0.0f,0.0f });
	//�X�v���C�g����
	PlayerComp = new PlayerUI();
	ActorManager::GetInstance()->AttachActor("Player");
	player_shadow = ActorManager::GetInstance()->SearchActor("Player");
	ActorManager::GetInstance()->AttachActor("Enemy");
	enemy_shadow = ActorManager::GetInstance()->SearchActor("Enemy");
	ActorManager::GetInstance()->AttachActor("Crystal");
	crystal_shadow = ActorManager::GetInstance()->SearchActor("Crystal");
	ActorManager::GetInstance()->AttachActor("ClearCrystal");
	goal_shadow = ActorManager::GetInstance()->SearchActor("ClearCrystal");
	goal_shadow->SetPosition(enemy_shadow->GetPosition());
	for (int i = 0; i < 10; i++) {
		ActorManager::GetInstance()->AttachBullet("Red");
	}
	for (int i = 0; i < 10; i++) {
		ActorManager::GetInstance()->AttachBullet("Green");
	}
	Object3d* Sky{};
	Sky = new Object3d();
	Sky->SetModel(ModelManager::GetIns()->GetModel(ModelManager::skydome));
	Sky->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	Sky->Initialize();
	skydome.reset(Sky);

	TouchableObject* Ground{};
	Ground = new TouchableObject();
	Ground->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Ground));
	Ground->SetPosition(XMFLOAT3(-50, 0, 50));
	Ground->SetScale(XMFLOAT3(5, 5, 5));
	//Ground->SetColor(XMFLOAT4(0.5f, 0.5f, 0.5f,1.0f))
	//Ground->SetRotation(XMFLOAT3(0, 180, 0));
	ground.reset(Ground);

	//TouchableObject* GoalItem_{};
	//GoalItem_ = new TouchableObject();
	//GoalItem_->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Goal));
	//GoalItem_->SetPosition(XMFLOAT3(0, 0, 0));
	//GoalItem_->SetScale(XMFLOAT3(1, 1, 1));
	////Ground->SetColor(XMFLOAT4(0.5f, 0.5f, 0.5f,1.0f))
	//GoalItem_->SetRotation(XMFLOAT3(-90, 0, 0));
	//GoalItem.reset(GoalItem_);

	Sprite* _clear = nullptr;
	_clear = Sprite::Create(ImageManager::Clear, { 0,0 });
	Clear.reset(_clear);

	Sprite* _Over = nullptr;
	_Over = Sprite::Create(ImageManager::Over, { 0,0 });
	Over.reset(_Over);

	Sprite* _Screen = nullptr;
	_Screen = Sprite::Create(ImageManager::SceneCover, { 0,0 });
	Screen[0].reset(_Screen);
	Sprite* _Screen2 = nullptr;
	_Screen2 = Sprite::Create(ImageManager::SceneCover, { 0,600 });
	Screen[1].reset(_Screen2);

	Gauge::LoadTexture(0, L"Resources/2d/PlayUI/Lock.png");
	Gauge* _Gauge = nullptr;
	_Gauge = Gauge::Create(0, { 0,0 });
	Demo.reset(_Gauge);
	//Demo->SetColor(XMFLOAT4{ 1,1,1,0.2f });

	//�X�v���C�g����
	Sprite* Effect_ = Sprite::Create(ImageManager::Black, { 0.0f,0.0f });
	Effect.reset(Effect_);
	Effect->SetColor({ 1,1,1,alpha });

	Sprite* IntroWord_1 = Sprite::Create(ImageManager::Intro01, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
	IntroWord[0].reset(IntroWord_1);
	Sprite* IntroWord_2 = Sprite::Create(ImageManager::Intro02, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
	IntroWord[1].reset(IntroWord_2);
	Sprite* IntroWord_3 = Sprite::Create(ImageManager::Intro03, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
	IntroWord[2].reset(IntroWord_3);
	Sprite* IntroWord_4 = Sprite::Create(ImageManager::Intro04, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
	IntroWord[3].reset(IntroWord_4);
	Sprite* IntroWord_5 = Sprite::Create(ImageManager::Intro05, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
	IntroWord[4].reset(IntroWord_5);
	Sprite* IntroWord_6 = Sprite::Create(ImageManager::Intro06, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
	IntroWord[5].reset(IntroWord_6);



	PauseUI* pause_ui = new PauseUI();
	pauseUi.reset(pause_ui);
	camera->SetTarget(player_shadow->GetPosition());

	postEffect = new PostEffect();
	postEffect->Initialize();
	miniMap = new  MiniMap();
	miniMap->Initialize();
	distance.x = sinf(angle * (PI / 180)) * 13.0f;
	distance.y = cosf(angle * (PI / 180)) * 13.0f;


	dis.x = sinf(angle * (PI / 180)) * 13.0f;
	dis.y = cosf(angle * (PI / 180)) * 13.0f;

}
//�J������
void PlayScene::Finalize() {
	//�R���̃��f���̃f���[�g
	ActorManager::GetInstance()->Finalize();
}
//�X�V
void PlayScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_P)) {
		ActorManager::GetInstance()->AttachBullet("Red");
	}
	if (crystal_shadow->GetPause()) {
		crystal_shadow->Update();
		return;
	}
	if (clear) {

		ResultCamera(count);
		count++;

		ActorManager::GetInstance()->ResultUpdate(count);
		ParticleManager::GetInstance()->Update();
		if (input->TriggerButton(Input::A)) {
			SceneManager::GetInstance()->ChangeScene("MAP");
		}
		return;
	}
	if (Intro) {
		IntroCamera(count);
		if (Change) {
			if (frame < 1.0f) {
				frame += 0.01f;
			} else {
				Change = false;
			}
			alpha = Ease(In, Cubic, frame, 1, 0);
			Effect->SetColor({ 1,1,1,alpha });
		}
		goal_shadow->SetIsActive(false);
		ActorManager::GetInstance()->IntroUpdate(count);
		skydome->Update();
		ground->Update();
		if (count > 1200) {
			Effect->SetColor({ 1,1,1,0 });
			count = 0;
			Intro = false;
		}
		count += speed;
		if (input->PushButton(input->START)) {
			speed = 2;
		}
		if (count % 200 == 0) {
			if (nowWord != 5) {
				nowWord++;
			}
		}
		return;
	}
	if (enemy_shadow->GetPause()) {

		const float rnd_vel = 0.4f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		ParticleManager::GetInstance()->Add(0, 120, enemy_shadow->GetPosition(), vel, XMFLOAT3(), 1.2f, 0.0f);
		ParticleManager::GetInstance()->Update();

		finishTime++;
		if (finishTime > 200) {

			enemy_shadow->SetPause(false);
			enemy_shadow->SetCommand(Actor::DEAD);
		}

		return;
	}
	CameraUpda();
	if (pause) {
		pauseUi->Update();
		if (!pauseUi->GetEase()) {
			if (input->TriggerButton(input->A) ||
				input->TriggerKey(DIK_SPACE)) {
				switch (pauseUi->GetBar()) {
				case 0:
					SceneManager::GetInstance()->ChangeScene("TITLE");
				case 1:
					break;
				case 2:
					pauseUi->Reset();
					pause = false;
					break;
				default:
					break;
				}
			}
			return;
		}
		return;
	}
	if (input->TriggerButton(input->START)) {
		pause = true;
	}
	ActorManager::GetInstance()->Update();
	ParticleManager::GetInstance()->Update();
	skydome->Update();
	ground->Update();
#pragma region "Clear"
	if (!enemy_shadow->GetIsActive()) {
		goal_shadow->SetIsActive(true);
		if (goal_shadow->GetPause()) {
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

}

void PlayScene::CameraUpda() {
	Input* input = Input::GetInstance();
	if (Reset) {
		ResetCamera();
		return;
	}
	if (input->TiltPushStick(Input::R_RIGHT) || input->TiltPushStick(Input::R_LEFT)) {
		if (input->TiltPushStick(Input::R_RIGHT)) {
			angle -= 3;
		}
		if (input->TiltPushStick(Input::R_LEFT)) {
			angle += 3;
		}
		dis.x = sinf(angle * (PI / 180)) * 13.0f;
		dis.y = cosf(angle * (PI / 180)) * 13.0f;
		distance.x = Ease(In, Quad, 0.6f, distance.x, dis.x);
		distance.y = Ease(In, Quad, 0.6f, distance.y, dis.y);

	}
	if (input->TriggerButton(Input::LT)) {
		if (!Reset) {
			angleframe = 0.0f;
			firstangle = angle;
			endangle = player_shadow->GetRotation().y;
			firstdis = distance;
			Reset = true;
		}
	}
	player_shadow->SetAngle(angle);
	crystal_shadow->SetAngle(angle);
	camera->SetTarget(player_shadow->GetCameraPos(angle));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + 10.0f,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}

void PlayScene::IntroCamera(int Timer) {
	if (Timer <= 720) {
		if (speed == 1) {
			angle += 0.5f;
			if (IntroHight > 10.0f) {
				IntroHight -= 0.075f;
			} else {
				IntroHight = 10.0f;
			}
		} else {
			angle += 1.0f;
			if (IntroHight > 10.0f) {
				IntroHight -= 0.150f;
			} else {
				IntroHight = 10.0f;
			}
		}
	}

	dis.x = sinf(angle * (PI / 180)) * 13.0f;
	dis.y = cosf(angle * (PI / 180)) * 13.0f;
	distance.x = Ease(In, Quad, 0.6f, distance.x, dis.x);
	distance.y = Ease(In, Quad, 0.6f, distance.y, dis.y);
	player_shadow->SetAngle(angle);
	camera->SetTarget(player_shadow->GetCameraPos(angle));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,IntroHight,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}

void PlayScene::ResultCamera(int Timer) {
	if (Timer <= 720) {
		angle += 0.5f;
	}

	dis.x = sinf(angle * (PI / 180)) * 13.0f;
	dis.y = cosf(angle * (PI / 180)) * 13.0f;
	distance.x = Ease(In, Quad, 0.6f, distance.x, dis.x);
	distance.y = Ease(In, Quad, 0.6f, distance.y, dis.y);
	player_shadow->SetAngle(angle);
	camera->SetTarget(goal_shadow->GetPosition());
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + 10.0f,player_shadow->GetPosition().z + distance.y });
	camera->Update();

}

//�`��
void PlayScene::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	//postEffect->PreDrawScene(dxCommon->GetCmdList());
	//ImGui::Begin("playscene");
	//ImGui::SliderFloat("bulletX", &distance.x, 0, 360);
	//ImGui::SliderFloat("bulletY", &distance.y, 0, 360);
	//ImGui::SliderFloat("Anglet", &angle, 0, 360);
	//ImGui::End();
	Object3d::PreDraw();
	skydome->Draw();
	ground->Draw();
	//�w�i�p
	ActorManager::GetInstance()->Draw(dxCommon);
	ParticleManager::GetInstance()->Draw(dxCommon->GetCmdList());
	Sprite::PreDraw();
	if (Change) {
		Effect->Draw();
	}
	if (clear) {
		Clear->Draw();
	}
	if (pause) {
		pauseUi->Draw();
	}
	if (Intro) {
		Screen[0]->Draw();
		Screen[1]->Draw();
		IntroWord[nowWord]->Draw();
	}
	if (Result) {
		Screen[0]->Draw();
		Screen[1]->Draw();
		Clear->Draw();
	}
	if (GameOver) {
		Over->Draw();
	}
	//Demo->Draw();

	//miniMap->PreDraw();
	//miniMap->Draw(dxCommon->GetCmdList());
	//miniMap->PostDraw();

	//postEffect->PostDrawScene(dxCommon->GetCmdList());

	//dxCommon->PreDraw();
	//ImGui::Begin("test");
	//float F = FPSManager::GetInstance()->GetFps();
	//ImGui::SliderFloat("fps", &F, 120, 0);
	//ImGui::SliderFloat("cameraPos.y", &A, 35000, 0);
	//ImGui::Unindent();
	//ImGui::End();
	//postEffect->Draw(dxCommon->GetCmdList());
	dxCommon->PostDraw();

}

void PlayScene::ResetCamera() {
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

	dis.x = sinf(angle * (PI / 180)) * 13.0f;
	dis.y = cosf(angle * (PI / 180)) * 13.0f;

	distance.x = Ease(In, Quad, angleframe, firstdis.x, dis.x);
	distance.y = Ease(In, Quad, angleframe, firstdis.y, dis.y);
	camera->SetTarget(player_shadow->GetCameraPos(angle));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + 10.0f,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}



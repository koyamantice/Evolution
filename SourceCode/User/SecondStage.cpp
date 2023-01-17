#include"SecondStage.h"
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

void SecondStage::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	BattleInit();


	//スプライト生成
	ActorManager::GetInstance()->AttachActor("Player");
	ActorManager::GetInstance()->AttachActor("Enemy_Bee");
	ActorManager::GetInstance()->AttachActor("ClearCrystal");

	player_shadow = ActorManager::GetInstance()->SearchActor("Player");
	enemy_shadow = ActorManager::GetInstance()->SearchActor("Enemy");
	goal_shadow = ActorManager::GetInstance()->SearchActor("ClearCrystal");
	goal_shadow->SetPosition(enemy_shadow->GetPosition());
	goal_shadow->SetIsActive(false);

	for (int i = 0; i < 10; i++) {
		ActorManager::GetInstance()->AttachBullet("Red");
	}
	for (int i = 0; i < 10; i++) {
		ActorManager::GetInstance()->AttachBullet("Green");
	}

	Sprite* _clear = nullptr;
	_clear = Sprite::Create(ImageManager::Clear, { 0,0 });
	Clear.reset(_clear);

	Sprite* _Over = nullptr;
	_Over = Sprite::Create(ImageManager::Over, { 0,0 });
	Over.reset(_Over);


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

	camera->SetTarget(player_shadow->GetPosition());

	partMan = new ParticleManager();
	partMan->Initialize(ImageManager::charge);

	postEffect = new PostEffect();
	postEffect->Initialize();

	distance.x = sinf(angle * (XM_PI / 180)) * 13.0f;
	distance.y = cosf(angle * (XM_PI / 180)) * 13.0f;

	dis.x = distance.x;
	dis.y = distance.y;
}
//開放処理
void SecondStage::Finalize() {
	//３ｄのモデルのデリート
	ActorManager::GetInstance()->Finalize();
}
//更新
void SecondStage::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	if (clear) {
		ResultCamera(count);
		count++;
		ActorManager::GetInstance()->ResultUpdate(count);
		const float rnd_vel = 1.4f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		partMan->Add(100, goal_shadow->GetPosition(), vel, XMFLOAT3(), 1.2f, 0.0f, { 1,1,1,1 }, { 1,1,1,0 });
		partMan->Update();
		if (input->TriggerButton(Input::A)|| input->TriggerButton(Input::B)) {
			scene_changer->ChangeStart();
		}
		scene_changer->ChangeScene("MAP");
		SkydomeUpdate();
		ground->Update();
		return;
	}
	if (Intro) {
		IntroCamera(count);
		if (scene_first_change) {
			if (introFrame < 1.0f) {
				introFrame += 0.01f;
			} else {
				scene_first_change = false;
			}
			filter_alpha = Ease(In, Cubic, introFrame, 1, 0);
			filter_first->SetColor({ 1,1,1,filter_alpha });
		}
		count += speed;
		if (count > 900) {
			ActorManager::GetInstance()->IntroUpdate(9999);
			filter_first->SetColor({ 1,1,1,0 });
			count = 0;
			Intro = false;
		}
		player_shadow->IntroUpdate(count);
		enemy_shadow->IntroUpdate(count);
		SkydomeUpdate();
		ground->Update();

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

		SkydomeSunny(finishTime);

		if (finishTime > 200) {
			enemy_shadow->SetPause(false);
			enemy_shadow->SetCommand(Actor::DEAD);
		}
		return;
	}
	CameraUpda();
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

	ActorManager::GetInstance()->Update();
	SkydomeUpdate();
	ground->Update();
	partMan->Update();
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

void SecondStage::CameraUpda() {
	Input* input = Input::GetInstance();
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
			firstdis = distance;
			Reset = true;
		}
	}
	player_shadow->SetAngle(angle);
	camera->SetTarget(player_shadow->GetCameraPos(angle, 7));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + hight,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}

void SecondStage::IntroCamera(int Timer) {











	dis.x = sinf(angle * (XM_PI / 180)) * 13.0f;
	dis.y = cosf(angle * (XM_PI / 180)) * 13.0f;
	distance.x = Ease(In, Quad, 0.6f, distance.x, dis.x);
	distance.y = Ease(In, Quad, 0.6f, distance.y, dis.y);
	camera->SetTarget({0,5,0});
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,10,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}

void SecondStage::ResultCamera(int Timer) {
	if (Timer <= 720) {
		angle += 0.5f;
	}

	//player_shadow->SetAngle(angle);
	camera->SetTarget(goal_shadow->GetPosition());
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + hight,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}


//描画
void SecondStage::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	//postEffect->PreDrawScene(dxCommon->GetCmdList());
	//ImGui::Begin("SecondStage");
	//ImGui::SliderFloat("bulletX", &distance.x, 0, 360);
	//ImGui::SliderFloat("bulletY", &distance.y, 0, 360);
	//ImGui::SliderFloat("Anglet", &angle, 0, 360);
	//ImGui::End();
	Object3d::PreDraw();
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
		//IntroWord[nowWord]->Draw();
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

void SecondStage::ResetCamera() {
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
	camera->SetTarget(player_shadow->GetCameraPos(angle));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + 10.0f,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}

float SecondStage::RandHeight(const float& base) {
	const float& rnd_vel = 0.95f;
	float Rand= (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	float itr = 0;
	itr = base + Rand;

	return itr;
}

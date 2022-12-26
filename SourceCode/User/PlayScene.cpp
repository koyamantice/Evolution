#include "PlayScene.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "input.h"
#include <string>
#include "ModelManager.h"
#include <TisGame.h>
#include "Player.h"
#include "PlayerUI.h"
#include"Enemy.h"
#include <SourceCode/Common/Easing.h>
#include"ActorManager.h"

void PlayScene::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	//スプライト生成
	ActorManager::GetInstance()->AttachActor("Player");
	player_shadow = ActorManager::GetInstance()->SearchActor("Player");
	ActorManager::GetInstance()->AttachActor("Enemy");
	enemy_shadow = ActorManager::GetInstance()->SearchActor("Enemy");
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
	Ground->SetPosition(XMFLOAT3(-50, -0.5f, 50));
	Ground->SetScale(XMFLOAT3(5, 5, 5));
	//Ground->SetColor(XMFLOAT4(0.5f, 0.5f, 0.5f,1.0f))
	//Ground->SetRotation(XMFLOAT3(0, 180, 0));
	ground.reset(Ground);

	Object3d* Patch{};
	Patch = new Object3d();
	Patch->Initialize();
	Patch->SetModel(ModelManager::GetIns()->GetModel(ModelManager::grassPatch));
	Patch->SetPosition(XMFLOAT3(0, 0.5f, 30));
	//Patch->SetBillboard(true);
	Patch->SetScale(XMFLOAT3(10, 10, 10));
	grassPatch.reset(Patch);
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
		Camecon[i]->SetSize({ 84,84 });
		Camecon[i]->SetScale(1.0f);
		Camecon[i]->SetAnchorPoint({ 0.5f,0.5f });
	}
	Camecon[0]->SetPosition({ 540.0f,530.0f });
	Camecon[1]->SetPosition({ 540.0f,530.0f });
	Camecon[2]->SetPosition({ 540.0f,530.0f });
	Camecon[3]->SetPosition({ 740.0f,530.0f });
	Camecon[4]->SetPosition({ 740.0f,530.0f });
	Camecon[5]->SetPosition({ 640.0f,530.0f });
	const int p = 2;
	for (int i = 0; i < 2; i++) {
		Sprite* Rockon_[2]{};
		Rockon_[i] = Sprite::Create(ImageManager::Rockon, { 0,0 });
		Rockon[i].reset(Rockon_[i]);
		int number_index_y = i / p;
		int number_index_x = i % p;
		Rockon[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * w, static_cast<float>(number_index_y) * h },
			{ static_cast<float>(w), static_cast<float>(h) });
		Rockon[i]->SetSize({ 84,84 });
		Rockon[i]->SetScale(1.0f);
		Rockon[i]->SetAnchorPoint({ 0.5f,0.5f });
	}

	Sprite* _clear = nullptr;
	_clear = Sprite::Create(ImageManager::Clear, { 0,0 });
	Clear.reset(_clear);

	Sprite* _Over = nullptr;
	_Over = Sprite::Create(ImageManager::Over, { 0,0 });
	Over.reset(_Over);

	//スプライト生成
	Sprite* Effect_ = Sprite::Create(ImageManager::Black, { 0.0f,0.0f });
	FeedBlack.reset(Effect_);
	FeedBlack->SetColor({ 1,1,1,feedAlpha });

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

	postEffect = new PostEffect();
	postEffect->Initialize();

	distance.x = sinf(angle * (XM_PI / 180)) * 13.0f;
	distance.y = cosf(angle * (XM_PI / 180)) * 13.0f;

	dis.x = distance.x;
	dis.y = distance.y;
}
//開放処理
void PlayScene::Finalize() {
	//３ｄのモデルのデリート
	ActorManager::GetInstance()->Finalize();
}
//更新
void PlayScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	if (clear) {
		ResultCamera(count);
		count++;
		ActorManager::GetInstance()->ResultUpdate(count);
		ParticleManager::GetInstance()->Update();
		if (input->TriggerButton(Input::A)) {
			Change = true;
		}
		Feed("MAP");
		skydome->Update();
		ground->Update();

		return;
	}
	if (Intro) {
		IntroCamera(count);
		if (Change) {
			if (introFrame < 1.0f) {
				introFrame += 0.01f;
			} else {
				Change = false;
			}
			feedAlpha = Ease(In, Cubic, introFrame, 1, 0);
			FeedBlack->SetColor({ 1,1,1,feedAlpha });
		}
		goal_shadow->SetIsActive(false);
		ActorManager::GetInstance()->IntroUpdate(count);
		skydome->Update();
		ground->Update();
		if (count > 1200) {
			FeedBlack->SetColor({ 1,1,1,0 });
			count = 0;
			Intro = false;
		}
		count += speed;
		if (input->PushButton(input->START)) {
			if (count %2 ==1) {
				count--;
			}
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
		}
		return;
	}
	if (input->TriggerButton(input->START)) {
		pause = true;
	}
	animafrate++;
	if (animafrate == 30) {
		if (animation < 2&&animation > 0) {
			animation += vec;
		} else if(animation == 2) {
			animation = 1;
			vec *= -1;
		} else if(animation == 0) {
			animation = 1;
			vec *= -1;
		}
		if (tapanima==3) {
			tapanima = 4;
		} else {
			tapanima = 3;
		}
		animafrate = 0;
	}
	if (cameraExplanation) {
			CameraAlpha *= 0.8f;
			for (int i = 0; i < 6;i++) {
				Camecon[i]->SetColor({ 1,1,1,CameraAlpha });
			}
	}
	ActorManager::GetInstance()->Update();
	ParticleManager::GetInstance()->Update();
	skydome->Update();
	ground->Update();
#pragma region "Clear"
	if (!enemy_shadow->GetIsActive()) {
		goal_shadow->SetIsActive(true);
		if (goal_shadow->GetPause()||player_shadow->GetPause()) {
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
	grassPatch->Update();
}

void PlayScene::CameraUpda() {
	Input* input = Input::GetInstance();
	if (Reset) {
		ResetCamera();
		return;
	}
	if (input->TiltPushStick(Input::R_RIGHT) || input->TiltPushStick(Input::R_LEFT)) {
		if (!cameraExplanation) {
			cameraExplanation = true;
		}
		
		if (input->TiltPushStick(Input::R_RIGHT)) {
			angle -= 3;
		}
		if (input->TiltPushStick(Input::R_LEFT)) {
			angle += 3;
		}
		dis.x = sinf(angle * (XM_PI / 180)) * 13.0f;
		dis.y = cosf(angle * (XM_PI / 180)) * 13.0f;
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
	//crystal_shadow->SetAngle(angle);
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

	dis.x = sinf(angle * (XM_PI / 180)) * 13.0f;
	dis.y = cosf(angle * (XM_PI / 180)) * 13.0f;
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

	//player_shadow->SetAngle(angle);
	camera->SetTarget(goal_shadow->GetPosition());
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + 10.0f,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}

//描画
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
	//grassPatch->Draw();
	//背景用
	ActorManager::GetInstance()->Draw(dxCommon);
	ParticleManager::GetInstance()->Draw(dxCommon->GetCmdList());
	Sprite::PreDraw();
	if (Change) {
		FeedBlack->Draw();
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
	} else {
			Camecon[animation]->Draw();
			Camecon[tapanima]->Draw();
			Camecon[5]->Draw();
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

	dis.x = sinf(angle * (XM_PI / 180)) * 13.0f;
	dis.y = cosf(angle * (XM_PI / 180)) * 13.0f;

	distance.x = Ease(In, Quad, angleframe, firstdis.x, dis.x);
	distance.y = Ease(In, Quad, angleframe, firstdis.y, dis.y);
	camera->SetTarget(player_shadow->GetCameraPos(angle));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + 10.0f,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}
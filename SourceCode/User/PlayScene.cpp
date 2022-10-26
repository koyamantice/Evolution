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
	//背景スプライト生成
	//sprite[back] = Sprite::Create(ImageManager::TITLE, { 0.0f,0.0f });
	//スプライト生成
	PlayerComp = new PlayerUI();
	ActorManager::GetInstance()->AttachActor("Player");
	player_shadow = ActorManager::GetInstance()->SearchActor("Player");
	ActorManager::GetInstance()->AttachActor("Enemy");
	enemy_shadow= ActorManager::GetInstance()->SearchActor("Enemy");
	ActorManager::GetInstance()->AttachActor("Crystal");
	crystal_shadow= ActorManager::GetInstance()->SearchActor("Crystal");
	for (int i = 0; i < 30;i++) {
		ActorManager::GetInstance()->AttachBullet("Red");
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
	Ground->SetPosition(XMFLOAT3(-50,0,50));
	Ground->SetScale(XMFLOAT3(5, 5, 5));
	//Ground->SetColor(XMFLOAT4(0.5f, 0.5f, 0.5f,1.0f));

	//Ground->SetRotation(XMFLOAT3(0, 180, 0));
	ground.reset(Ground);

	Sprite* _clear = nullptr;
	_clear = Sprite::Create(ImageManager::Clear, clearPos);
	Clear.reset(_clear);

	Sprite* _Vignette = nullptr;
	_Vignette = Sprite::Create(ImageManager::Vignette, {0,0});
	Vignette.reset(_Vignette);
	Vignette->SetColor(XMFLOAT4{0.0f,0.0f,0.0f,0.2f});


	Gauge::LoadTexture(0, L"Resources/2d/Lock.png");
	Gauge* _Gauge = nullptr;
	_Gauge = Gauge::Create(0, { 0,0 });
	Demo.reset(_Gauge);
	//Demo->SetColor(XMFLOAT4{ 1,1,1,0.2f });


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
//開放処理
void PlayScene::Finalize() {
	//３ｄのモデルのデリート
	ActorManager::GetInstance()->Finalize();
}
//更新
void PlayScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_P)) {
		ActorManager::GetInstance()->AttachBullet("Bullet");

	}
	CameraUpda();
	if (input->TriggerButton(input->Y)) {
		SceneManager::GetInstance()->ChangeScene("DEBUG");
	}
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
	if (input->TriggerButton(input->START)||
		input->TriggerKey(DIK_1)) {
		pause = true;
	}
	
	ActorManager::GetInstance()->Update();
	ParticleManager::GetInstance()->Update();
	skydome->Update();
	ground->Update();

#pragma region "Clear"
	if (enemy_shadow->GetHp()< 0) {
		if (!clear) {
			clear = true;
		}
	}
	if (clear) {
		if (Cframe >= 1.0f) {
			Cframe = 1.0f;
			if (input->PushKey(DIK_SPACE)) {
				SceneManager::GetInstance()->ChangeScene("TITLE");
			}

		} else {
			Cframe += 1.0f / 90.0f;
		}
		clearPos.y = Ease(InOut,Elastic,Cframe,-720,0);
		Clear->SetPosition(clearPos);
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
			angle -= 1;
		}
		if (input->TiltPushStick(Input::R_LEFT)) {
			angle += 1;
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
	//背景用
	ActorManager::GetInstance()->Draw(dxCommon);
	ParticleManager::GetInstance()->Draw(dxCommon->GetCmdList());	    
	Sprite::PreDraw();
	Vignette->Draw();
	if (clear) {
		Clear->Draw();
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

void PlayScene::ResetCamera() {
		player_shadow->SetCanMove(false);

		if (angleframe < 1.0f) {
			angleframe += 0.05f;
		} else {
			Reset = false;
			player_shadow->SetCanMove(true);
			angleframe = 1.0f;
			distance.x=dis.x;
			distance.x=dis.y;
		}

		angle = Ease(In, Quad,angleframe,firstangle,endangle);

		dis.x = sinf(angle * (PI / 180)) * 13.0f;
		dis.y = cosf(angle * (PI / 180)) * 13.0f;

		distance.x = Ease(In, Quad, angleframe, firstdis.x, dis.x);
		distance.y = Ease(In, Quad, angleframe, firstdis.y, dis.y);
		camera->SetTarget(player_shadow->GetCameraPos(angle));
		camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + 10.0f,player_shadow->GetPosition().z + distance.y });
		camera->Update();
}



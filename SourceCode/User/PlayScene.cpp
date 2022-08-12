#include "PlayScene.h"
#include"SceneManager.h"
#include "AudioManager.h"
#include "input.h"
#include<string>
#include "DebugText.h"
#include"ImageManager.h"
#include"ModelManager.h"
#include <TisGame.h>
#include"Player.h"
#include"Enemy.h"
#include <SourceCode/Common/Easing.h>
#include"ActorManager.h"

void PlayScene::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	//背景スプライト生成
	//sprite[back] = Sprite::Create(ImageManager::TITLE, { 0.0f,0.0f });
	//スプライト生成
	ActorManager::GetInstance()->AttachActor("Player");
	player_shadow = ActorManager::GetInstance()->SearchActor("Player");
	ActorManager::GetInstance()->AttachActor("Enemy");
	enemy_shadow= ActorManager::GetInstance()->SearchActor("Enemy");
	ActorManager::GetInstance()->AttachActor("Crystal");
	ActorManager::GetInstance()->AttachActor("Bullet");
	ActorManager::GetInstance()->AttachActor("Bullet");
	ActorManager::GetInstance()->AttachActor("Bullet");


	Object3d* Sky{};
	Sky = new Object3d();
	Sky->SetModel(ModelManager::GetIns()->GetModel(ModelManager::skydome));
	//Sky->SetModel(ModelManager::GetIns()->GetModel(ModelManager::skydome));
	Sky->Initialize();
	skydome.reset(Sky);

	TouchableObject* Ground{};
	Ground = new TouchableObject();
	Ground->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Ground));
	Ground->SetPosition(XMFLOAT3(0,0,0));
	Ground->SetRotation(XMFLOAT3(0, 180, 0));
	ground.reset(Ground);

	Sprite* _clear = nullptr;
	_clear = Sprite::Create(ImageManager::Clear, clearPos);
	Clear.reset(_clear);

	Sprite* _Vignette = nullptr;
	_Vignette = Sprite::Create(ImageManager::Vignette, {0,0});
	Vignette.reset(_Vignette);
	Vignette->SetColor(XMFLOAT4{1,1,1,0.2f});

	PauseUI* pause_ui = new PauseUI();
	pauseUi.reset(pause_ui);
	camera->SetTarget(player_shadow->GetPosition());
}
//開放処理
void PlayScene::Finalize() {
	//３ｄのモデルのデリート
	ActorManager::GetInstance()->Finalize();
}
//更新
void PlayScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	CameraUpda();
	if (input->TriggerButton(input->Button_Y)) {
		SceneManager::GetInstance()->ChangeScene("DEBUG");
	}
	if (pause) {
		pauseUi->Update();
		if (input->TriggerButton(input->Button_A)) {
			pauseUi->Reset();
			pause = false;
		}
		return;
	}
	if (input->TriggerButton(input->Start)||
		input->TriggerKey(DIK_1)) {
		pause = true;
	}
	
	ActorManager::GetInstance()->Update();

	skydome->Update();
	ground->Update();
	if (input->PushKey(DIK_0)) {
		int a = 0;
		a++;
	}
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
}
//描画
void PlayScene::Draw(DirectXCommon* dxCommon) {
	Object3d::PreDraw();
	skydome->Draw();
	ground->Draw();
	//背景用

	ActorManager::GetInstance()->Draw();
	if (pause) {
		pauseUi->Draw();
	}

	Sprite::PreDraw();
	Vignette->Draw();
	if (clear) {
		Clear->Draw();
	}
}

void PlayScene::CameraUpda() {
	Input* input = Input::GetInstance();
//if((input->RightTiltStick(input->Right)||input->RightTiltStick(input->Left))
//	||(input->PushKey(DIK_RIGHT)||input->PushKey(DIK_LEFT))|| input->TriggerButton(input->Button_LS)){
//	if (input->RightTiltStick(input->Right)) {
//		angle-=1;
//	}
//	if (input->RightTiltStick(input->Left)) {
//		angle+=1;
//	}
//
//	if (input->TriggerButton(input->Button_LB)){
//		player_shadow->SetCanMove(false);
//		angle = player_shadow->GetRotation().y;
//	} else {
//		player_shadow->SetCanMove(true);
//	}
//
//	dis.x=sinf(angle * (PI / 180)) * 15.0f;
//	dis.y=cosf(angle*(PI/180)) * 15.0f;
//}

	distance.x=Ease(In,Quad,0.5f,distance.x,dis.x);
	distance.y=Ease(In,Quad,0.5f,distance.y,dis.y);

	player_shadow->SetAngle(angle);
	
	camera->SetTarget(XMFLOAT3{ player_shadow->GetPosition().x,player_shadow->GetPosition().y,player_shadow->GetPosition().z });
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x+distance.x,player_shadow->GetPosition().y + 10.0f,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}


#include"MapScene.h"
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

void MapScene::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	//背景スプライト生成
	//スプライト生成
	PlayerComp = new PlayerUI();
	ActorManager::GetInstance()->AttachActor("Player");
	player_shadow = ActorManager::GetInstance()->SearchActor("Player");

	TouchableObject* Ground{};
	Ground = new TouchableObject();
	Ground->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Ground));
	Ground->SetPosition(XMFLOAT3(-50, 0, 50));
	Ground->SetScale(XMFLOAT3(5, 5, 5));
	//Ground->SetRotation(XMFLOAT3(0, 180, 0));
	ground.reset(Ground);

	Object3d* Patch{};
	Patch = new Object3d();
	Patch->Initialize();
	Patch->SetModel(ModelManager::GetIns()->GetModel(ModelManager::grassPatch));
	Patch->SetPosition(XMFLOAT3(0, 0, 0));
	Patch->SetBillboard(true);
	Patch->SetScale(XMFLOAT3(5, 5, 5));
	grassPatch.reset(Patch);

	PauseUI* pause_ui = new PauseUI();
	pauseUi.reset(pause_ui);
	camera->SetTarget(player_shadow->GetPosition());

	//postEffect = new PostEffect();
	//postEffect->Initialize();

}
//開放処理
void MapScene::Finalize() {
	//３ｄのモデルのデリート
	ActorManager::GetInstance()->Finalize();
}
//更新
void MapScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_0)) {
		int a = 0;
		a++;
	}
	CameraUpda();
	ActorManager::GetInstance()->Update();
	ParticleManager::GetInstance()->Update();
	ground->Update();
	grassPatch->Update();

}

void MapScene::CameraUpda() {
	Input* input = Input::GetInstance();
	player_shadow->SetAngle(angle);
	camera->SetTarget(XMFLOAT3{ player_shadow->GetPosition().x,player_shadow->GetPosition().y,player_shadow->GetPosition().z - 3 });
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + 13.0f,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}

//描画
void MapScene::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	//postEffect->PreDrawScene(dxCommon->GetCmdList());
	Object3d::PreDraw();
	//ground->Draw();
	grassPatch->Draw();

	//背景用
	ActorManager::GetInstance()->Draw(dxCommon);
	ParticleManager::GetInstance()->Draw(dxCommon->GetCmdList());
	Sprite::PreDraw();
	//postEffect->PostDrawScene(dxCommon->GetCmdList());
	//dxCommon->PreDraw();
	//postEffect->Draw(dxCommon->GetCmdList());
	dxCommon->PostDraw();

}



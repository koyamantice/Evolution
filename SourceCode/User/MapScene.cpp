#include"MapScene.h"
#include"SceneManager.h"
#include "AudioManager.h"
#include "input.h"
#include<string>
#include "DebugText.h"
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
	//�w�i�X�v���C�g����
	//�X�v���C�g����
	PlayerComp = new PlayerUI();
	ActorManager::GetInstance()->AttachActor("Player");
	player_shadow = ActorManager::GetInstance()->SearchActor("Player");
	for (int i = 0; i < 30; i++) {
		ActorManager::GetInstance()->AttachActor("Bullet");
	}
	TouchableObject* Ground{};
	Ground = new TouchableObject();
	Ground->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Ground));
	Ground->SetPosition(XMFLOAT3(0, 0, 0));
	Ground->SetScale(XMFLOAT3(5, 5, 5));
	Ground->SetColor(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));

	Ground->SetRotation(XMFLOAT3(0, 180, 0));
	ground.reset(Ground);

	PauseUI* pause_ui = new PauseUI();
	pauseUi.reset(pause_ui);
	camera->SetTarget(player_shadow->GetPosition());

	//postEffect = new PostEffect();
	//postEffect->Initialize();

}
//�J������
void MapScene::Finalize() {
	//�R���̃��f���̃f���[�g
	ActorManager::GetInstance()->Finalize();
}
//�X�V
void MapScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_0)) {
		int a = 0;
		a++;
	}
	CameraUpda();
	ActorManager::GetInstance()->Update();
	//ParticleManager::GetInstance()->Update();
	ground->Update();
}

void MapScene::CameraUpda() {
	Input* input = Input::GetInstance();
	player_shadow->SetAngle(angle);
	camera->SetTarget(XMFLOAT3{ player_shadow->GetPosition().x,player_shadow->GetPosition().y,player_shadow->GetPosition().z - 3 });
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + distance.x,player_shadow->GetPosition().y + 13.0f,player_shadow->GetPosition().z + distance.y });
	camera->Update();
}

//�`��
void MapScene::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	//postEffect->PreDrawScene(dxCommon->GetCmdList());
	Object3d::PreDraw();
	ground->Draw();
	//�w�i�p
	ActorManager::GetInstance()->Draw(dxCommon);
	ParticleManager::GetInstance()->Draw(dxCommon->GetCmdList());
	Sprite::PreDraw();
	//postEffect->PostDrawScene(dxCommon->GetCmdList());
	//dxCommon->PreDraw();
	//postEffect->Draw(dxCommon->GetCmdList());
	dxCommon->PostDraw();

}


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
void PlayScene::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	//�w�i�X�v���C�g����
	//sprite[back] = Sprite::Create(ImageManager::TITLE, { 0.0f,0.0f });
	//�X�v���C�g����
	Actor* Act_[Chr_Max]{};
	Act_[MPlayer] = new Player();
	Act_[MPlayer]->Initialize(ModelManager::GetIns()->GetModel(ModelManager::Player));
	actor[MPlayer].reset(Act_[MPlayer]);
	Object3d* Sky{};
	Sky = new Object3d();
	Sky->SetModel(ModelManager::GetIns()->GetModel(ModelManager::skydome));
	Sky->Initialize();
	skydome.reset(Sky);

	Object3d* Ground{};
	Ground = new Object3d();
	Ground->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Ground));
	Ground->Initialize();
	ground.reset(Ground);
	camera->SetTarget(actor[MPlayer]->GetPosition());
}
//�J������
void PlayScene::Finalize() {
	//�R���̃��f���̃f���[�g
}
//�X�V
void PlayScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	XMFLOAT3 plaPos = actor[MPlayer]->GetPosition();
	camera->SetTarget(XMFLOAT3{plaPos.x,plaPos.y,plaPos.z});
	camera->SetEye(XMFLOAT3{ plaPos.x,plaPos.y+10.0f,plaPos.z+10.0f});
	camera->Update();
	actor[MPlayer]->Update();
	skydome->Update();
	ground->Update();
	if (input->PushKey(DIK_0)) {
		int a = 0;
		a++;
	}

}
//�`��
void PlayScene::Draw(DirectXCommon* dxCommon) {
	ImGui::Begin("test");
	//ImGui::SliderFloat("cameraPos.y", &cameraPos.y, 30, 0);
	ImGui::Unindent();
	ImGui::End();
	//Sprite::PreDraw();
	Object3d::PreDraw();
	skydome->Draw();
	ground->Draw();
	//�w�i�p
	actor[MPlayer]->Draw();
}


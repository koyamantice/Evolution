#include "TitleScene.h"
#include"SceneManager.h"
#include "AudioManager.h"
#include "input.h"
#include<string>
#include "DebugText.h"
#include"ImageManager.h"
#include"ModelManager.h"
#include <TisGame.h>
#include"ActorManager.h"

void TitleScene::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	//�w�i�X�v���C�g����
	Sprite* sprite_ = Sprite::Create(ImageManager::Title, { 0.0f,0.0f });
	UI[Button].reset(sprite_);
	//�X�v���C�g����
//	ActorManager::GetInstance()->AttachActor("Player");
}
//�J������
void TitleScene::Finalize() {
	//�R���̃��f���̃f���[�g
	ActorManager::GetInstance()->Finalize();
}
//�X�V
void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();

	camera->Update();
	ActorManager::GetInstance()->Update();
	if (input->PushKey(DIK_0)) {
		int a = 0;
		a++;
	}
	if (input->TriggerKey(DIK_SPACE)||input->PushButton(input->Button_A)) {
		SceneManager::GetInstance()->ChangeScene("PLAY");
	}
	if (input->TriggerKey(DIK_RETURN) || input->PushButton(input->Button_B)) {
		SceneManager::GetInstance()->ChangeScene("DEBUG");
	}

}
//�`��
void TitleScene::Draw(DirectXCommon* dxCommon) {
	//ImGui::Begin("test");
	//ImGui::SliderFloat("cameraPos.y", &cameraPos.y, 30, 0);
	//ImGui::Unindent();
	//ImGui::End();
	Sprite::PreDraw();
	UI[Button]->Draw();
	Object3d::PreDraw();
	ActorManager::GetInstance()->Draw();
}


#include "TitleScene.h"
#include"SceneManager.h"
#include "AudioManager.h"
#include "input.h"
#include<string>
#include "DebugText.h"
#include"ImageManager.h"
#include"ModelManager.h"
#include <TisGame.h>
#include"Player.h"
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	//背景スプライト生成
	Sprite* sprite_ = Sprite::Create(ImageManager::Test, { 0.0f,0.0f });
	UI[Button].reset(sprite_);
	//スプライト生成

}
//開放処理
void TitleScene::Finalize() {
	//３ｄのモデルのデリート
}
//更新
void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();

	camera->Update();

	if (input->PushKey(DIK_0)) {
		int a = 0;
		a++;
	}
	if (input->PushKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("PLAY");
	}
}
//描画
void TitleScene::Draw(DirectXCommon* dxCommon) {
	//ImGui::Begin("test");
	//ImGui::SliderFloat("cameraPos.y", &cameraPos.y, 30, 0);
	//ImGui::Unindent();
	//ImGui::End();
	Sprite::PreDraw();
	UI[Button]->Draw();
	Object3d::PreDraw();

}


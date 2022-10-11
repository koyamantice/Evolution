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
#include"Easing.h"
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	//背景スプライト生成
	Sprite* sprite_ = Sprite::Create(ImageManager::Title, { 0.0f,0.0f });
	UI[Button].reset(sprite_);
	//スプライト生成
	Sprite* Effect_ = Sprite::Create(ImageManager::Black, { 0.0f,0.0f });
	Effect.reset(Effect_);
	Effect->SetColor({1,1,1,alpha});
}
//開放処理
void TitleScene::Finalize() {
	//３ｄのモデルのデリート
	ActorManager::GetInstance()->Finalize();
}
//更新
void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	C = input->GetRPosX();
	A = input->GetRPosY();

	camera->Update();
	ActorManager::GetInstance()->Update();
	if (input->TiltStick(Input::R_RIGHT)) {
		int a = 0;
		a++;
	}
	if (input->TriggerKey(DIK_SPACE)||input->TriggerButton(input->A)) {
		Change = true;
		SceneName="PLAY";
	}else if (input->TriggerKey(DIK_RETURN) || input->TriggerButton(input->B)) {
		Change = true;
		SceneName = "DEBUG";
	} else if (input->TriggerKey(DIK_M) || input->TriggerButton(input->Y)) {
		Change = true;
		SceneName = "MAP";
	}



	Feed(SceneName);
}
//描画
void TitleScene::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	//ImGui::Begin("test");
	//ImGui::SliderFloat("cameraPos.y", &C, 35000, 0);
	//ImGui::SliderFloat("cameraPos.y", &A, 35000, 0);
	//ImGui::Unindent();
	//ImGui::End();
	Sprite::PreDraw();
	UI[Button]->Draw();
	Object3d::PreDraw();
	ActorManager::GetInstance()->Draw(dxCommon);
	Sprite::PreDraw();
	Effect->Draw();
	dxCommon->PostDraw();

}

void TitleScene::Feed(const std::string& sceneName) {
	if (Change) {
		if (frame < 1.6f) {
			frame += 0.02f;
		} else {
			SceneManager::GetInstance()->ChangeScene(sceneName);
		}
		alpha = Ease(In, Cubic, frame, 0, 1);
		Effect->SetColor({ 1,1,1,alpha });
	}

}


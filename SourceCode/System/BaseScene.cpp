#include "BaseScene.h"
#include <SourceCode/Common/Easing.h>
#include"SceneManager.h"

void BaseScene::InitCommon(DirectXCommon* dxCommon) {
	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	Object2d::SetCamera(camera);
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);
	//音の挿入
	// ライト生成
	lightGroup = LightGroup::Create();
	// 3Dオブエクトにライトをセット
	Object3d::SetLightGroup(lightGroup);
	//この順番で書かないと例外スローになります
	// デバイスをセット
	FBXObject3d::SetDevice(dxCommon->GetDev());
	// カメラをセット
	FBXObject3d::SetCamera(camera);
	// グラフィックスパイプライン生成
	FBXObject3d::CreateGraphicsPipeline();
	//パーティクルにカメラセット
	ParticleManager::GetInstance()->SetCamera(camera);



	//導入枠
	Sprite* _Screen = nullptr;
	_Screen = Sprite::Create(ImageManager::SceneCover, { 0,0 });
	Screen[0].reset(_Screen);
	Sprite* _Screen2 = nullptr;
	_Screen2 = Sprite::Create(ImageManager::SceneCover, { 0,600 });
	Screen[1].reset(_Screen2);
	//スプライト生成
	Sprite* Effect_ = Sprite::Create(ImageManager::Black, { 0.0f,0.0f });
	Effect.reset(Effect_);
	Effect->SetColor({ 1,1,1,alpha });

	//
	PauseUI* pause_ui = new PauseUI();
	pauseUi.reset(pause_ui);


}

void BaseScene::Feed(const std::string& sceneName) {
	static float frame = 0.0f;
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

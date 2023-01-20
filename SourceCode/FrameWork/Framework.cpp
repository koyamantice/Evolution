#include "Framework.h"
#include "FbxLoader.h"
#include <SourceCode/FrameWork/ActorManager.h>

void Framework::Run() {
	
	//初期化
	Initialize(dxcommon);
	while (true)//ゲームループ
	{
		if (FPSManager::GetInstance()->Run()) {
			//毎フレーム更新
			Update(dxcommon);
			//終了リクエストが来たら抜ける
			if (IsEndRequst()) {
				break;
			}
			//描画
			Draw(dxcommon);
		}
	}
	//ゲームの終了
	Finalize();

}

void Framework::Initialize(DirectXCommon* dxCommon) {
	winApp = new WinApp();
	winApp->Initialize();
	dxcommon = new DirectXCommon();
	dxcommon->Initialize(winApp);
	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize(winApp);

	audio = AudioManager::GetInstance();
	audio->Initialize();	
	
	fps = FPSManager::GetInstance();
	fps->Init();
	//スプライト関係
	// スプライト静的初期化
	Sprite::StaticInitialize(dxcommon->GetDev(), dxcommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	ImageManager::GetIns()->Load2D();

	// ライト静的初期化
	LightGroup::StaticInitialize(dxcommon->GetDev());
	//テクスチャ初期化
	Object2d::StaticInitialize(dxcommon->GetDev(), dxcommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	ImageManager::GetIns()->LoadTex2D();
	Object3d::StaticInitialize(dxcommon->GetDev(), dxcommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	ActorManager::GetInstance()->Initialize();

	// マウスカーソルの非表示
	ShowCursor(TRUE);
	//XorShiftの初期化
	xorShift = XorShift::GetInstance();
	XorShift::GetInstance()->initrand((unsigned int)time(NULL));
	//シーンマネージャー
	// FBX関連静的初期化
	FbxLoader::GetInstance()->Initialize(dxcommon->GetDev());
	ModelManager::GetIns()->Initialize();

	postEffect = new PostEffect();
	postEffect->Initialize();
}

void Framework::Finalize() {
	ModelManager::GetIns()->Finalize();
	Sprite::Finalize();
	Object2d::Finalize();
	LightGroup::Finalize();
	SceneManager::GetInstance()->Finalize();
	input->GetInstance()->Finalize();
	delete sceneFactory_;
	dxcommon->Finalize();
	dxcommon->Reset();
	delete dxcommon;
	winApp->Finalize();
	delete winApp;
}

void Framework::Update(DirectXCommon* dxCommon) {
	input->Update();
	XorShift::GetInstance()->initrand((unsigned int)time(NULL));
	XorShift::GetInstance()->init_xor128((unsigned long)time(NULL));
	if (winApp->ProcessMessage() || input->TriggerKey(DIK_ESCAPE) || SceneManager::GetInstance()->IsEndRequst()) {
		endResquest_ = true;
		return;
	}
	SceneManager::GetInstance()->Update(dxCommon);
}


void Framework::Draw(DirectXCommon* dxCommon) {
	postEffect->PreDrawScene(dxCommon->GetCmdList());
	SceneManager::GetInstance()->Draw(dxCommon);
	postEffect->PostDrawScene(dxCommon->GetCmdList());

	dxCommon->PreDraw();
	postEffect->Draw(dxCommon->GetCmdList());
	dxCommon->PostDraw();

}

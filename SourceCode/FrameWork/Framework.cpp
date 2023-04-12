#include "Framework.h"
#include "FbxLoader.h"
#include <SourceCode/FrameWork/ActorManager.h>
#include <Singleton.h>

void Framework::Run() {
	
	//初期化
	Initialize(dxCommon_);
	while (true)//ゲームループ
	{
		if (FPSManager::GetInstance()->Run()) {
			//毎フレーム更新
			Update(dxCommon_);
			//終了リクエストが来たら抜ける
			if (IsEndRequst()) {
				break;
			}
			//描画
			Draw(dxCommon_);
		}
	}
	//ゲームの終了
	Finalize();

}

void Framework::Initialize(DirectXCommon* dxCommon) {
	winApp_ = new WinApp();
	winApp_->Initialize();
	dxCommon_ = new DirectXCommon();
	dxCommon_->Initialize(winApp_);
	// 入力の初期化
	// マウスカーソルの非表示
	ShowCursor(TRUE);
	input_->Initialize(winApp_);
	//Fps設定
	fpsManager_->Initialize();

	// ライト静的初期化
	LightGroup::StaticInitialize(dxCommon_->GetDev());

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon_->GetDev(), dxCommon_->GetCmdList(), WinApp::window_width, WinApp::window_height);
	imageManager_->Load2D();
	//オブジェクト2D初期化
	Object2d::StaticInitialize(dxCommon_->GetDev(), dxCommon_->GetCmdList(), WinApp::window_width, WinApp::window_height);
	imageManager_->LoadTex2D();
	//オブジェクト3D初期化
	Object3d::StaticInitialize(dxCommon_->GetDev(), dxCommon_->GetCmdList(), WinApp::window_width, WinApp::window_height);
	// FBX関連静的初期化
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDev());
	ModelManager::GetInstance()->Initialize();
	
	//アクターマネージャーの初期化
	ActorManager::GetInstance()->Initialize();

	//XorShiftの初期化
	xorShift_->initrand((unsigned int)time(NULL));
}

void Framework::Finalize() {
	ModelManager::GetInstance()->Finalize();
	SceneManager::GetInstance()->Finalize();
	Input::GetInstance()->Finalize();
	Sprite::Finalize();
	Object2d::Finalize();
	LightGroup::Finalize();
	SingletonFinalizer::finalize();
	delete sceneFactory_;
	dxCommon_->Finalize();
	dxCommon_->Reset();
	delete dxCommon_;
	winApp_->Finalize();
	delete winApp_;
}

void Framework::Update(DirectXCommon* dxCommon) {
	Input::GetInstance()->Update();
	XorShift::GetInstance()->initrand((unsigned int)time(NULL));
	XorShift::GetInstance()->init_xor128((unsigned long)time(NULL));
	if (winApp_->ProcessMessage() || input_->TriggerKey(DIK_ESCAPE) || SceneManager::GetInstance()->IsEndRequst()) {
		endResquest_ = true;
		return;
	}
	SceneManager::GetInstance()->Update(dxCommon);
}


void Framework::Draw(DirectXCommon* dxCommon) {
	SceneManager::GetInstance()->Draw(dxCommon);
}

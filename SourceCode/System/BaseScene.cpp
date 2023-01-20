#include "BaseScene.h"
#include <SourceCode/Common/Easing.h>
#include"SceneManager.h"

void BaseScene::InitCommon(DirectXCommon* dxCommon) {
	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	// ライト生成
	lightGroup = LightGroup::Create();
	//Object2dの初期化
	Object2d::SetCamera(camera);
	//Object3dの初期化
	Object3d::StaticInitializeCommon(camera, lightGroup);
	//FBXの初期化
	FBXObject3d::StaticInitializeCommon(dxCommon->GetDev(), camera);
	//音の挿入
	
	//パーティクルにカメラセット
	ParticleManager::CreateCommon(dxCommon->GetDev(), camera, dxCommon->GetCmdList());
	//
	ImageManager::GetIns()->LoadParticle();

	//シーン切り替えの初期化
	SceneChanger* scene_changer_ = new SceneChanger();
	scene_changer_->Init();
	scene_changer.reset(scene_changer_);
}


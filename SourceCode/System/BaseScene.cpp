#include "BaseScene.h"
#include <SourceCode/Common/Easing.h>
#include"SceneManager.h"

void BaseScene::InitCommon(DirectXCommon* dxCommon) {
	// カメラ生成
	camera = std::make_unique<DebugCamera>(WinApp::window_width, WinApp::window_height);
	// ライト生成
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	
	//Object2dの初期化
	Object2d::SetCamera(camera.get());
	
	//Object3dの初期化
	Object3d::StaticInitializeCommon(camera.get(), lightGroup.get());
	
	//FBXの初期化
	FBXObject3d::StaticInitializeCommon(dxCommon->GetDev(), camera.get());

	//パーティクルにカメラセット
	ParticleManager::CreateCommon(dxCommon->GetDev(), camera.get(), dxCommon->GetCmdList());
	//パーティクルで必要なテクスチャを読み込む
	ImageManager::GetIns()->LoadParticle();

	//シーン切り替えの初期化
	scene_changer = std::make_unique <SceneChanger>();
	scene_changer->Init();
	scene_changer->InitOver();

}


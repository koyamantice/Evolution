#include"MSecondStage.h"

void MSecondStage::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	BattleInit();
	TorchSetup(Touch::FireColor::f_blue);

	//導入はボスステージじゃないので不要
	battle_intro = false;

	//ゲームアクターの生成をします。
	ActorManager::GetInstance()->AttachActor("Player");
	ActorManager::GetInstance()->AttachActor("ClearCrystal");
	for (int i = 0; i < kGnormNum; i++) {
		ActorManager::GetInstance()->AttachBullet("Red");
	}
	for (int i = 0; i < kGnormNum; i++) {
		ActorManager::GetInstance()->AttachBullet("Green");
	}

	//シーン内で必要なアクターを参照します。
	player_shadow = ActorManager::GetInstance()->SearchActor("Player");
	goal_shadow = ActorManager::GetInstance()->SearchActor("ClearCrystal");


	//スプライト生成
	Sprite* _clear = nullptr;
	_clear = Sprite::Create(ImageManager::Clear, { 0,0 });
	Clear.reset(_clear);

	Sprite* _Over = nullptr;
	_Over = Sprite::Create(ImageManager::Over, { 0,0 });
	Over.reset(_Over);


	//カメラの初期化
	camera->SetTarget(player_shadow->GetPosition());
	camera_distance.x = sinf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;
	camera_distance.z = cosf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;
	e_camera_distance.x = camera_distance.x;
	e_camera_distance.z = camera_distance.z;
	camera->SetEye({
		player_shadow->GetPosition().x + camera_distance.x,
		player_shadow->GetPosition().y + camera_hight,
		player_shadow->GetPosition().z + camera_distance.z
		});

	//ポストエフェクトの初期化
	postEffect = new PostEffect();
	postEffect->Initialize();

	//パーティクルの初期化
	particleEmitter = std::make_unique <ParticleEmitter>(ImageManager::charge);
}
//開放処理
void MSecondStage::Finalize() {
	//３ｄのモデルのデリート
	ActorManager::GetInstance()->Finalize();
}
//更新
void MSecondStage::Update(DirectXCommon* dxCommon) {
	if (ClearUpdate()) { return; }
	if (PauseUpdate()) { return; }
	//操作説明の更新処理
	DescriptionUpdate();
	//アクターすべての更新処理
	ActorManager::GetInstance()->Update();
	//カメラの更新処理
	CameraUpda();
	//ステージの更新処理
	FieldUpdate();
	//パーティクルの更新処理
	particleEmitter->Update();
}



void MSecondStage::ResultCamera(int Timer) {
	camera->SetTarget(goal_shadow->GetPosition());
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + camera_distance.x,player_shadow->GetPosition().y + camera_hight,player_shadow->GetPosition().z + camera_distance.z });
	camera->Update();
}

bool MSecondStage::ClearUpdate() {
	if (stage_clear) {
		const float rnd_vel = 0.5f;
		particleEmitter->AddCommon(100, goal_shadow->GetPosition(), rnd_vel, 0, 1.2f, 0.0f, { 1,1,0.5f,1 }, { 1,1,1,0.3f });
		particleEmitter->Update();

		if (input->TriggerButton(Input::A) || input->TriggerButton(Input::B)) {
			scene_changer->ChangeStart();
		}
		scene_changer->ChangeScene("SECONDSTAGE");
		ResultCamera(0);
		FieldUpdate();
		return true;
	}
	if (goal_shadow->GetPause()) {
		battle_result = true;
		stage_clear = true;
	}
	return false;
}


//描画
void MSecondStage::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	Object3d::PreDraw();
	skydome->Draw();
	ground->Draw();
	for (std::unique_ptr<Touch>& torch : torchs) {
		torch->Draw();
	}
	ActorManager::GetInstance()->Draw(dxCommon);
	particleEmitter->Draw(alphaBle);
	Sprite::PreDraw();
	if (scene_first_change) {
		filter_first->Draw();
	}
	if (stage_clear) {
		Clear->Draw();
	}
	if (battle_intro) {
		screens[0]->Draw();
		screens[1]->Draw();
	}
	if (battle_result) {
		screens[0]->Draw();
		screens[1]->Draw();
		Clear->Draw();
	}
	if (gameover) {
		Over->Draw();
	}
	if (pause) {
		pauseUi->Draw();
	}
	scene_changer->Draw();
	dxCommon->PostDraw();
}

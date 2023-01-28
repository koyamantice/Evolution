#include"MSecondStage.h"

void MSecondStage::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	BattleInit();
	TorchSetup(Touch::FireColor::f_blue);

	//導入はボスステージじゃないので不要
	battle_intro = true;

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
	goal_shadow->SetIsActive(false);
	for (int i = 0; i < kMaxNestNum; i++) {
		ActorManager::GetInstance()->AttachActor("Honey");
		honey_[i] = ActorManager::GetInstance()->SearchActor("Honey");
		honey_[i]->SetIsRefer(true);
	}
	honey_[0]->SetPosition({ -20.0f,0,20.0f});
	honey_[1]->SetPosition({ 0.0f,0,-20.0f });
	honey_[2]->SetPosition({ 20.0f,0,20.0f});




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
	//導入部分の処理
	if (IntroUpdate()) { return; }
	//クリア後の処理
	if (ClearUpdate()) { return; }
	//ポーズ処理の更新
	if (PauseUpdate()) { return; }
	//操作説明の更新処理
	DescriptionUpdate();
	//アクターすべての更新処理
	ActorManager::GetInstance()->Update();
	//カメラの更新処理
	CameraUpda();
	//ステージの更新処理
	FieldUpdate();
	//蜂蜜の更新処理
	HoneyUpdate();
	//パーティクルの更新処理
	particleEmitter->Update();
}

bool MSecondStage::IntroUpdate() {
	//フェードイン機能
	if (scene_first_change) {
		if (feedin_frame <= 1.0f) {
			feedin_frame += 1.0f / feedin_frame_max;
		} else {
			battle_intro = false;
			scene_first_change = false;
		}
		filter_alpha = Ease(Out, Cubic, feedin_frame, 1, 0);
		filter_first->SetColor({ 1,1,1,filter_alpha });
		ActorManager::GetInstance()->IntroUpdate(0,"", kSecondScene);
		CameraUpda();
		FieldUpdate();
		return true;
	} else {
		return false;
	}
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
	if (!goal_shadow->GetIsActive()) {
		if (honey_[kLeftNest]->GetCommand() == Actor::APPROCH &&
			honey_[kMiddleNest]->GetCommand() == Actor::APPROCH &&
			honey_[kRightNest]->GetCommand() == Actor::APPROCH) {
			//finish_time++;
			//if (finish_time > finish_time_Max) {
				goal_shadow->SetIsActive(true);
			//}
			//return true;
			return false;

		}
	}
	if (goal_shadow->GetPause()) {
		battle_result = true;
		stage_clear = true;
	}

	return false;
}

void MSecondStage::HoneyUpdate() {
	for (int i = 0; i < kMaxNestNum; i++) {
		if (honey_[i]->GetCommand() == Actor::APPROCH) {
			const float rnd_vel = 0.1f;
			particleEmitter->AddCommon(60, honey_[i]->GetPosition(), rnd_vel, 0, 1.2f, 0.0f, { 1.0f,1.0f,0.0f,0.8f }, { 1,1,0,0 });
			particleEmitter->Update();
		}
	}
}


//描画
void MSecondStage::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	
	BattleBackDraw();

	ActorManager::GetInstance()->Draw(dxCommon);

	BattleFrontDraw();

	dxCommon->PostDraw();
}

#include"MSecondStage.h"

void MSecondStage::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	BattleInit();
	TorchSetup(Touch::FireColor::f_blue);

	audioManager = std::make_unique<AudioManager>();

	audioManager->LoadWave("BGM/MSecondBattle.wav");
	audioManager->PlayWave("BGM/MSecondBattle.wav", 0.3f);
	audioManager->LoadWave("BGM/Clear.wav");

	
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

	const XMFLOAT2 kBasicAnchor = { 0.5f,0.5f };

	Sprite* _mission{};
	_mission = Sprite::Create(ImageManager::kMissionMsecond, { 640,100 });
	_mission->SetAnchorPoint(kBasicAnchor);
	mission_.reset(_mission);
	
	const XMFLOAT2 kBasicSize = { 64,64 };
	for (int i = 0; i < kHoneyNumMax; i++) {
		Sprite* _honey_get[kHoneyNumMax]{};
		_honey_get[i] = Sprite::Create(ImageManager::kMsecondNum, { 640-64,135 });
		honey_get_[i].reset(_honey_get[i]);
		int number_index_y = i / kHoneyNumMax;
		int number_index_x = i % kHoneyNumMax;
		honey_get_[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * kBasicSize.x, static_cast<float>(number_index_y) * kBasicSize.y },
			{ static_cast<float>(kBasicSize.x), static_cast<float>(kBasicSize.y) });
		
		honey_get_[i]->SetSize(kBasicSize);
		//中心座標にします。
		honey_get_[i]->SetAnchorPoint(kBasicAnchor);
	}


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
	s_eye = { player_shadow->GetPosition().x + camera_distance.x,player_shadow->GetPosition().y + camera_hight,player_shadow->GetPosition().z + camera_distance.z };

	s_target = player_shadow->GetCameraPos(camera_angle, camera_target);

	//ポストエフェクトの初期化
	postEffect = new PostEffect();
	postEffect->Initialize();

	//パーティクルの初期化
	particleEmitter = std::make_unique <ParticleEmitter>(ImageManager::nul);
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
	//ミッション情報の更新処理
	MissionUpdate();
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
	//操作説明の更新処理
	hud->Update();
	//パーティクルの更新処理
	particleEmitter->Update();
}

bool MSecondStage::IntroUpdate() {
	//フェードイン機能
	if (scene_first_change) {
		ActorManager::GetInstance()->IntroUpdate(0, "", kSecondScene);
		CameraUpda();
		FieldUpdate();
		if (feedin_frame <= 1.0f) {
			feedin_frame += 1.0f / feedin_frame_max;
		} else {
			battle_intro = false;
			scene_first_change = false;
		}
		filter_alpha = Ease(Out, Cubic, feedin_frame, 1, 0);
		filter_first->SetColor({ 1,1,1,filter_alpha });
		return true;
	} else {
		return false;
	}
}

bool MSecondStage::MissionUpdate() {
	if (isVisible_) {
		visible_timer_++;
		if (visible_timer_>= kVisibleTimerMax) {

			if (mission_ease_timer_<= 1.0f) {
				mission_ease_timer_ += 1.0f / kMissionEaseTimerMax;
			} else {
				isVisible_ = false;
			}
			mission_pos_ = mission_->GetPosition();
			number_pos_= honey_get_[nowOpenHoney]->GetPosition();

			mission_pos_.y = Ease(InOut, Elastic, mission_ease_timer_, 100,-100);
			number_pos_.y = Ease(InOut, Elastic, mission_ease_timer_, 135, -135);


			mission_->SetPosition(mission_pos_);
			for (int i = 0; i < kHoneyNumMax;i++) {
				honey_get_[i]->SetPosition(number_pos_);
			}
		}
	}
	if (pause) {
		isVisible_ = true;
		mission_pos_ = mission_->GetPosition();
		number_pos_ = honey_get_[nowOpenHoney]->GetPosition();
		visible_timer_ = 0;
		mission_ease_timer_ = 0;

		mission_pos_.y = 100;
		number_pos_.y = 135;
		mission_->SetPosition(mission_pos_);
		for (int i = 0; i < kHoneyNumMax; i++) {
			honey_get_[i]->SetPosition(number_pos_);
		}

	}
	return false;
}

void MSecondStage::ResultCamera(int Timer) {
	camera->SetTarget(player_shadow->GetPosition());
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + camera_distance.x,player_shadow->GetPosition().y + camera_hight,player_shadow->GetPosition().z + camera_distance.z });
	camera->Update();
}

void MSecondStage::SmashCamera(const float& Timer) {
	XMFLOAT3  target_honey_ = honey_[smash_honey_]->GetPosition();

	XMFLOAT3 e_eye = { target_honey_.x + camera_distance.x,target_honey_.y + camera_hight,target_honey_.z + camera_distance.z };
	XMFLOAT3 ease_target = target_honey_;
	XMFLOAT3 ease_eye = e_eye;
	float ease_time = Timer / 60.0f;

	if (ease_time <= 1.0f) {

		ease_target = {
		Ease(In,Linear,ease_time,s_target.x,target_honey_.x),
		Ease(In,Linear,ease_time,s_target.y,target_honey_.y),
		Ease(In,Linear,ease_time,s_target.z,target_honey_.z)
		};

		ease_eye = {
			Ease(In,Linear,ease_time,s_eye.x,e_eye.x),
			Ease(In,Linear,ease_time,s_eye.y,e_eye.y),
			Ease(In,Linear,ease_time,s_eye.z,e_eye.z)
		};
	}
	if(Timer>=90.0f){

		finish_time = 0;
		if (smash_honey_< kRightNest) {
			if (smash_honey_== kLeftNest) {
				audioManager->StopWave("BGM/MSecondBattle.wav");
				audioManager->PlayWave("BGM/Clear.wav", 0.3f);
			}
			smash_honey_++;
		} else {
			battle_result = true;
			stage_clear = true;
		}
		s_eye = e_eye;
		s_target = target_honey_;
	}

	camera->SetTarget(ease_target);
	camera->SetEye(ease_eye);
	camera->Update();

}

bool MSecondStage::ClearUpdate() {
	if (stage_clear) {

		if (input->TriggerButton(Input::A) || input->TriggerButton(Input::B)) {
			sceneChanger_->ChangeStart();
		}
		sceneChanger_->ChangeScene("SECONDSTAGE");
		ActorManager::GetInstance()->ResultUpdate(0);
		ResultCamera(0);
		FieldUpdate();
		return true;
	}
	if (!honey_[kLeftNest]->GetCanMove()&&
			!honey_[kMiddleNest]->GetCanMove()&&
			!honey_[kRightNest]->GetCanMove()) {
			finish_time++;
			player_shadow->SetCanMove(false);
			particleEmitter->Update();
			SmashCamera((float)(finish_time));
			ActorManager::GetInstance()->Update();
			FieldUpdate();

			return true;
	}

	return false;
}

void MSecondStage::HoneyUpdate() {

	nowOpenHoney = 0;
	for (int i = 0; i < kMaxNestNum; i++) {
		if (!honey_[i]->GetCanMove()) {
			nowOpenHoney++;
			const float rnd_vel = 0.2f;
			const float rnd_height = 0.3f;
			particleEmitter->AddParabo(50, honey_[i]->GetPosition(), rnd_height, rnd_vel, 1.5f, 0.0f, { 1.0f,1.0f,0.0f,0.8f }, { 1,1,0,0 });
			particleEmitter->Update();
		}
	}
}

void MSecondStage::DrawLocal() {
	if (!pause) {
		mission_->Draw();
		honey_get_[nowOpenHoney]->Draw();
	}
}


//描画
void MSecondStage::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	
	BattleBackDraw();

	ActorManager::GetInstance()->Draw(dxCommon);

	BattleFrontDraw(addBle);

	dxCommon->PostDraw();
}

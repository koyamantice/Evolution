#include "FirstStage.h"

void FirstStage::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	BattleInit();
	TorchSetup(Touch::FireColor::f_green);
	shineRain_ = new ShineRain();
	shineRain_->Init();
	//ゲームアクターの生成をします。
	ActorManager::GetInstance()->AttachActor("Player");
	ActorManager::GetInstance()->AttachActor("MashGhost");
	//ActorManager::GetInstance()->AttachActor("ClearCrystal");

	for (int i = 0; i < kGnormNum; i++) {
		ActorManager::GetInstance()->AttachBullet("Red");
	}
	for (int i = 0; i < kGnormNum; i++) {
		ActorManager::GetInstance()->AttachBullet("Green");
	}
	//シーン内で必要なアクターを参照します。
	player_shadow = ActorManager::GetInstance()->SearchActor("Player");
	enemy_shadow = ActorManager::GetInstance()->SearchActor("Enemy");



	//導入部分の言葉
	for(int i=0;i<intro_word_max;i++){ 
		Sprite* IntroWord_ = Sprite::Create(ImageManager::Intro01 + i, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
		IntroWord[i].reset(IntroWord_);	
	}

	//カメラの初期化
	camera_distance.x = sinf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;
	camera_distance.z = cosf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;
	player_shadow->SetAngle(camera_angle);
	camera->SetTarget(player_shadow->GetCameraPos(camera_angle, camera_target));
	camera->SetEye(XMFLOAT3{ 
		player_shadow->GetPosition().x + camera_distance.x,
		first_hight,
		player_shadow->GetPosition().z + camera_distance.z
		});
	camera->Update();

	//ポストエフェクトの初期化
	postEffect = new PostEffect();
	postEffect->Initialize();
	//
	audioManager = std::make_unique<AudioManager>();

	audioManager->LoadWave("BGM/battle.wav");
	audioManager->PlayWave("BGM/battle.wav", 0.5f);
	audioManager->LoadWave("SE/lastHit.wav");
	audioManager->LoadWave("BGM/Smash.wav");

	//パーティクルの初期化
	particleEmitter = std::make_unique <ParticleEmitter>(ImageManager::charge);
}
//更新
void FirstStage::Update(DirectXCommon* dxCommon) {
	//導入部分の処理
	if (IntroUpdate()) { return; }
	//クリア後の処理
	if (ClearUpdate()) { return; }
	//ポーズ処理の更新
	if (PauseUpdate()) { return; }
	//ゲームオーバーの処理
	GameOverUpdate();
	//アクターすべての更新処理
	ActorManager::GetInstance()->Update();
	//カメラの更新処理
	CameraUpda();
	//ステージの更新処理
	FieldUpdate();
	//操作説明の更新処理
	hud->Update();
	//パーティクルの更新処理
	particleEmitter->Update();
}

//描画
void FirstStage::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	//postEffect->PreDrawScene(dxCommon->GetCmdList());
	BattleBackDraw();
	//背景用
	ActorManager::GetInstance()->Draw(dxCommon);
	BattleFrontDraw(alphaBle,IntroWord[nowWord].get());
	shineRain_->Draw();
	dxCommon->PostDraw();
}

//開放処理
void FirstStage::Finalize() {
	//３ｄのモデルのデリート
	ActorManager::GetInstance()->Finalize();
}

bool FirstStage::IntroUpdate() {
	if (battle_intro) {
		//フェードイン機能
		if (scene_first_change) {
			if (feedin_frame <= 1.0f) {
				feedin_frame += 1.0f / feedin_frame_max;
			} else {
				scene_first_change = false;
			}
			filter_alpha = Ease(Out, Cubic, feedin_frame, 1, 0);
			filter_first->SetColor({ 1,1,1,filter_alpha });
			ActorManager::GetInstance()->IntroUpdate(intro_count);
			FieldUpdate();
			return true;
		}

		//スキップ
		if (intro_skip) {
			if (feedin_frame > 0.0f) {
				feedin_frame -= 1.0f / feedin_frame_max;
			} else {
				intro_skip = false;
				intro_count = intro_count_max;
				IntroCamera(intro_count_max);
				ActorManager::GetInstance()->IntroUpdate(intro_count / intro_count_max, "", kSecondScene);
				FieldUpdate();
			}
			filter_alpha = Ease(Out, Cubic, feedin_frame, 1, 0);
			filter_first->SetColor({ 1,1,1,filter_alpha });
			
			return true;
		}

		if (input->TriggerButton(input->START)||
			input->TriggerKey(DIK_SPACE)) {
			if (!intro_skip) {
				intro_skip = true;
			}
		}

		intro_count += intro_speed;

		//導入部分終了処理
		if (intro_count >= intro_count_max) {
			intro_count = 0;
			battle_intro = false;
			return true;
		}

		//導入の語りを分割しています.
		if ((int)intro_count % ((int)(intro_count_max / intro_speed) / intro_word_max) == 0) {
			if (nowWord != intro_word_max - 1) {
				nowWord++;
			}
		}

		IntroCamera(intro_count);
		ActorManager::GetInstance()->IntroUpdate(intro_count / intro_count_max);
		FieldUpdate();
		return true;
	}
	return false;
}

void FirstStage::IntroCamera(const float& Timer) {
	//着地してからの猶予
	const float delay = 0.9f;
	const float reaching_time = intro_count_max * delay;
	float hight = camera_hight;
	if (Timer/ reaching_time <= 1.0f) {
		camera_angle = Ease(In, Linear, Timer / reaching_time, DEGREE_MAX, 0);
		hight = Ease(In, Linear, Timer / reaching_time, first_hight, camera_hight);
	}
	if (Timer/ intro_count_max >= 1.0f) {
		camera_angle = 0;
		hight = camera_hight;
	}
	camera_distance.x = sinf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;
	camera_distance.z = cosf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;

	player_shadow->SetAngle(camera_angle);
	camera->SetTarget(player_shadow->GetCameraPos(camera_angle, camera_target));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + camera_distance.x,hight,player_shadow->GetPosition().z + camera_distance.z });
	camera->Update();
}

void FirstStage::DescriptionUpdate() {
}

void FirstStage::ResultCamera(const float& Timer) {
	camera->SetTarget(player_shadow->GetCameraPos(camera_angle, camera_target));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + camera_distance.x,player_shadow->GetPosition().y + camera_hight,player_shadow->GetPosition().z + camera_distance.z });
	camera->Update();
}

void FirstStage::SmashCamera(const float& Timer) {
	XMFLOAT3 s_eye = { player_shadow->GetPosition().x + camera_distance.x,player_shadow->GetPosition().y + camera_hight,player_shadow->GetPosition().z + camera_distance.z };
	XMFLOAT3 e_eye = { enemy_shadow->GetPosition().x + camera_distance.x,enemy_shadow->GetPosition().y + 25,enemy_shadow->GetPosition().z + camera_distance.z };
	XMFLOAT3 ease_target = enemy_shadow->GetPosition();
	XMFLOAT3 ease_eye = e_eye;
	float ease_time = Timer / (float)60.0f;

	if (ease_time<=1.0f) {

		ease_target = {
		Ease(In,Linear,ease_time,player_shadow->GetCameraPos(camera_angle, camera_target).x,enemy_shadow->GetPosition().x),
		Ease(In,Linear,ease_time,player_shadow->GetCameraPos(camera_angle, camera_target).y,enemy_shadow->GetPosition().y),
		Ease(In,Linear,ease_time,player_shadow->GetCameraPos(camera_angle, camera_target).z,enemy_shadow->GetPosition().z)
		};

		ease_eye = {
			Ease(In,Linear,ease_time,s_eye.x,e_eye.x),
			Ease(In,Linear,ease_time,s_eye.y,e_eye.y),
			Ease(In,Linear,ease_time,s_eye.z,e_eye.z)
		};
	}

	camera->SetTarget(ease_target);
	camera->SetEye(ease_eye);
	camera->Update();

}


bool FirstStage::ClearUpdate() {
	if (stage_clear) {
		//const float rnd_vel = 0.5f;
		//particleEmitter->AddCommon(100, goal_shadow->GetPosition(), rnd_vel, 0.0f, 1.2f, 0.0f, { 1,1,0.5f,1 }, { 1,1,1,0.3f });
		//particleEmitter->Update();
		shineRain_->Update();
		if (input->TriggerButton(Input::A) || input->TriggerButton(Input::B)) {
			scene_changer->ChangeStart();
		}
		ResultCamera(0);
		scene_changer->ChangeScene("MSECOND");
		ActorManager::GetInstance()->ResultUpdate(0);
		//パーティクルの更新処理
		particleEmitter->Update();
		FieldUpdate();
		return true;
	}
	if (enemy_shadow->GetPause()) {
		battle_result = true;
		if (player_shadow->GetCanMove()) {
			player_shadow->SetCanMove(false);
			audioManager->PlayWave("SE/lastHit.wav",0.5f);
			audioManager->StopWave("BGM/battle.wav");
		}
		const float rnd_vel = 0.4f;
		particleEmitter->AddCommon(120, enemy_shadow->GetPosition(), rnd_vel, 0, 1.2f, 0.0f, { 1,1,1,1 }, { 1,1,1,0 });
		particleEmitter->Update();
		finish_time++;

		SmashCamera((float)(finish_time));
		//enemy_shadow->Update();
		ActorManager::GetInstance()->Update();
		FieldUpdate();

		if (finish_time >=100.0f) {
			enemy_shadow->SetCanMove(false);
		}

		if (finish_time >= 200.0f) {
			stage_clear = true;
			audioManager->PlayWave("BGM/Smash.wav", 0.3f);
		}
		return true;
	}
	//if (finish_time > finish_time_Max) {
	//	if (goal_shadow->GetPause() || player_shadow->GetPause()) {
	//		battle_result = true;
	//		stage_clear = true;
	//	}
	//}
	return false;
}


bool FirstStage::GameOverUpdate() {
	if (ActorManager::GetInstance()->SearchNum("Bullet") <= 0) {
		gameover = true;
		player_shadow->SetCanMove(false);
		scene_changer->ChangeStart();
	}
	if (gameover) {
		scene_changer->ChangeGameOver();
	}
	return false;

}

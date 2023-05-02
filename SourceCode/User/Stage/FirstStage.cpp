#include "FirstStage.h"

void FirstStage::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	BattleInit();
	TorchSetup(Touch::FireColor::f_green);
	//ゲームアクターの生成をします。
	ActorManager::GetInstance()->AttachActor("Player");
	ActorManager::GetInstance()->AttachActor("MashGhost");
	lightGroup->SetPointLightActive(0, true);
	lightGroup->SetPointLightActive(1, true);
	lightGroup->SetPointLightActive(2, true);
	lightGroup->DefaultLightSetting();

	for (int i = 0; i < 3; i++) {
		lightGroup->SetPointLightPos(i, pointLightPos[i]);
	}


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
	for (int i = 0; i < intro_word_max; i++) {
		IntroWord[i] = Sprite::Create(ImageManager::Intro01 + i, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
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
	//オーディオ
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
	for (int i = 0; i < 3; i++) {
		lightGroup->SetPointLightPos(i, pointLightPos[i]);
	}
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
	DebugDraw();
	BattleBackDraw();
	//背景用
	ActorManager::GetInstance()->Draw(dxCommon);
	BattleFrontDraw(alphaBle, IntroWord[nowWord].get());
	dxCommon->PostDraw();
}

//開放処理
void FirstStage::Finalize() {
	//３ｄのモデルのデリート
	ActorManager::GetInstance()->Finalize();
}

bool FirstStage::DebugDraw() {
	if (!isDebug) { return false; }
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(1100, 0));
	//ImGui::SetNextWindowSize(ImVec2(180, 240));

	ImGui::Begin("scene");
	ImGui::Text("fps:%f", FPSManager::GetInstance()->GetFps());
	ImGui::Text("distance.x:%f", camera_distance.x);
	ImGui::Text("distance.z:%f", camera_distance.z);
	ImGui::SliderFloat("pointLightPos[0].x", &pointLightPos[0].x, -50.0f,50.0f);
	ImGui::SliderFloat("pointLightPos[0].y", &pointLightPos[0].y, -50.0f,50.0f);
	ImGui::SliderFloat("pointLightPos[0].z", &pointLightPos[0].z, -50.0f,50.0f);
	ImGui::SliderFloat("pointLightPos[1].x", &pointLightPos[1].x, -50.0f,50.0f);
	ImGui::SliderFloat("pointLightPos[1].y", &pointLightPos[1].y, -50.0f,50.0f);
	ImGui::SliderFloat("pointLightPos[1].z", &pointLightPos[1].z, -50.0f,50.0f);
	ImGui::SliderFloat("pointLightPos[2].x", &pointLightPos[2].x, -50.0f,50.0f);
	ImGui::SliderFloat("pointLightPos[2].y", &pointLightPos[2].y, -50.0f,50.0f);
	ImGui::SliderFloat("pointLightPos[2].z", &pointLightPos[2].z, -50.0f,50.0f);

	
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	return true;
}

bool FirstStage::IntroUpdate() {
	if (!battle_intro) { return false; }
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
		filter_alpha = Ease(Out, Cubic, feedin_frame, 1.0f, 0.0f);
		filter_first->SetColor({ 1,1,1,filter_alpha });
		return true;
	}

	if (input->TriggerButton(input->START) ||
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

void FirstStage::IntroCamera(const float timer) {
	//着地してからの猶予
	const float delay = 0.9f;
	const float reaching_time = intro_count_max * delay;
	float hight = camera_hight;
	if (timer / reaching_time <= 1.0f) {
		camera_angle = Ease(In, Linear, timer / reaching_time, DEGREE_MAX, 0);
		hight = Ease(In, Linear, timer / reaching_time, first_hight, camera_hight);
	}
	if (timer / intro_count_max >= 1.0f) {
		camera_angle = min(0, camera_angle);
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

void FirstStage::ResultCamera(const float timer) {
	camera->SetTarget(player_shadow->GetCameraPos(camera_angle, camera_target));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + camera_distance.x,player_shadow->GetPosition().y + camera_hight,player_shadow->GetPosition().z + camera_distance.z });
	camera->Update();
}

void FirstStage::SmashCamera(const float timer) {
	XMFLOAT3 s_eye = { player_shadow->GetPosition().x + camera_distance.x,player_shadow->GetPosition().y + camera_hight,player_shadow->GetPosition().z + camera_distance.z };
	XMFLOAT3 e_eye = { enemy_shadow->GetPosition().x + camera_distance.x,enemy_shadow->GetPosition().y + 25,enemy_shadow->GetPosition().z + camera_distance.z };
	XMFLOAT3 ease_target = enemy_shadow->GetPosition();
	XMFLOAT3 ease_eye = e_eye;
	float ease_time = timer / (float)60.0f;

	if (ease_time <= 1.0f) {

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
		if (input->TriggerButton(Input::A) || input->TriggerButton(Input::B)) {
			sceneChanger_->ChangeStart();
		}
		ResultCamera(0);
		sceneChanger_->ChangeScene("MSECOND");
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
			audioManager->PlayWave("SE/lastHit.wav", 0.5f);
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

		if (finish_time >= 100.0f) {
			enemy_shadow->SetCanMove(false);
		}

		if (finish_time >= 200.0f) {
			stage_clear = true;
			audioManager->PlayWave("BGM/Smash.wav", 0.3f);
		}
		return true;
	}
	return false;
}


bool FirstStage::GameOverUpdate() {
	if (ActorManager::GetInstance()->SearchNum("Bullet") <= 0) {
		gameover = true;
		player_shadow->SetCanMove(false);
		sceneChanger_->ChangeStart();
	}
	if (gameover) {
		sceneChanger_->ChangeGameOver();
	}
	return false;

}

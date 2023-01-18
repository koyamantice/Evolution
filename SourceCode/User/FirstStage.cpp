#include "FirstStage.h"

void FirstStage::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	BattleInit();
	TorchSetup(Touch::FireColor::f_green);

	//ゲームアクターの生成をします。
	ActorManager::GetInstance()->AttachActor("Player");
	ActorManager::GetInstance()->AttachActor("Enemy");
	ActorManager::GetInstance()->AttachActor("ClearCrystal");
	for (int i = 0; i < 10; i++) {
		ActorManager::GetInstance()->AttachBullet("Red");
	}
	for (int i = 0; i < 10; i++) {
		ActorManager::GetInstance()->AttachBullet("Green");
	}
	//シーン内で必要なアクターを参照します。
	player_shadow = ActorManager::GetInstance()->SearchActor("Player");
	enemy_shadow = ActorManager::GetInstance()->SearchActor("Enemy");
	goal_shadow = ActorManager::GetInstance()->SearchActor("ClearCrystal");
	goal_shadow->SetPosition(enemy_shadow->GetPosition());
	goal_shadow->SetIsActive(false);


	//カメラの操作説明
	const int w = 256;
	const int h = 256;
	const int l = 6;
	for (int i = 0; i < 6; i++) {
		Sprite* CameCon_[6]{};
		CameCon_[i] = Sprite::Create(ImageManager::CameCon, { 0,0 });
		Camecon[i].reset(CameCon_[i]);
		int number_index_y = i / l;
		int number_index_x = i % l;
		Camecon[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * w, static_cast<float>(number_index_y) * h },
			{ static_cast<float>(w), static_cast<float>(h) });
		Camecon[i]->SetSize({ 128,128 });
		Camecon[i]->SetScale(1.0f);
		Camecon[i]->SetAnchorPoint({ 0.5f,0.5f });
	}
	Camecon[0]->SetPosition({ base.x + Camecon[0]->GetSize().x,base.y });
	Camecon[1]->SetPosition({ base.x + Camecon[1]->GetSize().x,base.y });
	Camecon[2]->SetPosition({ base.x + Camecon[2]->GetSize().x,base.y });
	Camecon[3]->SetPosition({ base.x - Camecon[3]->GetSize().x,base.y });
	Camecon[4]->SetPosition({ base.x - Camecon[4]->GetSize().x,base.y });
	Camecon[5]->SetPosition(base);
	
	//コントローラー説明の表示
	const int w_2 = 512;
	const int p = 2;
	for (int i = 0; i < 2; i++) {
		Sprite* con_vis_[2]{};
		con_vis_[i] = Sprite::Create(ImageManager::Con_vis, { 200,360 });
		con_vis[i].reset(con_vis_[i]);
		int number_index_y = i / p;
		int number_index_x = i % p;
		con_vis[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * w_2, static_cast<float>(number_index_y) * h },
			{ static_cast<float>(w_2), static_cast<float>(h) });
		con_vis[i]->SetSize({ 128,64 });
		con_vis[i]->SetAnchorPoint({ 0.5f,0.5f });
	}
	con_vis[0]->SetPosition({70,520});
	con_vis[1]->SetPosition({70,600});

	//スプライト生成
	Sprite* _clear = nullptr;
	_clear = Sprite::Create(ImageManager::Clear, { 0,0 });
	Clear.reset(_clear);

	Sprite* _Over = nullptr;
	_Over = Sprite::Create(ImageManager::Over, { 0,0 });
	Over.reset(_Over);


	//導入部分の言葉
	for(int i=0;i<intro_word_max;i++){ 
		Sprite* IntroWord_ = Sprite::Create(ImageManager::Intro01 + i, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
		IntroWord[i].reset(IntroWord_);	
	}

	//カメラの初期化
	camera_distance.x = sinf(camera_angle * (XM_PI / 180)) * camera_radius;
	camera_distance.z = cosf(camera_angle * (XM_PI / 180)) * camera_radius;
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

	//パーティクルの初期化
	partMan = new ParticleManager();
	partMan->Initialize(ImageManager::charge);
}
//更新
void FirstStage::Update(DirectXCommon* dxCommon) {
	if (IntroUpdate()) { return; }
	//クリア後の処理
	if (ClearUpdate()) { return; }
	//ポーズ処理の更新
	if (PauseUpdate()) { return; }
	//ゲームオーバーの処理
	GameOverUpdate();
	//操作説明の更新処理
	DescriptionUpdate();
	//アクターすべての更新処理
	ActorManager::GetInstance()->Update();
	//カメラの更新処理
	CameraUpda();
	//ステージの更新処理
	FieldUpdate();
	//パーティクルの更新処理
	partMan->Update();
}

//描画
void FirstStage::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	//postEffect->PreDrawScene(dxCommon->GetCmdList());
	Object3d::PreDraw();
	skydome->Draw();
	ground->Draw();
	for (std::unique_ptr<Touch>& torch : torchs) {
		torch->Draw();
	}
	//背景用
	ActorManager::GetInstance()->Draw(dxCommon);
	partMan->Draw(alphaBle);
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
		IntroWord[nowWord]->Draw();
	} else {
		con_vis[0]->Draw();
		con_vis[1]->Draw();
		if (!stage_clear) {
			Camecon[animation]->Draw();
			Camecon[tapanima]->Draw();
			Camecon[5]->Draw();
		}
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

		intro_count += intro_speed;

		//導入部分終了処理
		if (intro_count >= intro_count_max) {
			intro_count = 0;
			battle_intro = false;
			return true;
		}
		//倍速機能
		if (input->TriggerButton(input->START)) {
			if ((int)intro_count % 2 == 1) { intro_count -= 1; }
			intro_speed = intro_speed_max;
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
	float hight = 0.0f;
	if (Timer/ reaching_time <= 1.0f) {
		camera_angle = Ease(In, Linear, Timer / reaching_time, DEGREE_MAX, 0);
		hight = Ease(In, Linear, Timer / reaching_time, first_hight, camera_hight);
	}

	camera_distance.x = sinf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;
	camera_distance.z = cosf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;

	player_shadow->SetAngle(camera_angle);
	camera->SetTarget(player_shadow->GetCameraPos(camera_angle, camera_target));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + camera_distance.x,hight,player_shadow->GetPosition().z + camera_distance.z });
	camera->Update();
}

void FirstStage::ResultCamera(const float& Timer) {

	player_shadow->SetAngle(camera_angle);
	camera->SetTarget(goal_shadow->GetPosition());
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + camera_distance.x,player_shadow->GetPosition().y + camera_hight,player_shadow->GetPosition().z + camera_distance.y });
	camera->Update();
}

void FirstStage::DescriptionUpdate() {
	animafrate++;
	if (animafrate == 30) {
		if (animation < 2 && animation > 0) {
			animation += vec;
		} else if (animation == 2) {
			animation = 1;
			vec *= -1;
		} else if (animation == 0) {
			animation = 1;
			vec *= -1;
		}
		if (tapanima == 3) {
			tapanima = 4;
		} else {
			tapanima = 3;
		}
		animafrate = 0;
	}

	if (camera_explanation) {
		if (camera_frame < 1.0f) {
			camera_frame += 0.005f;
		} else {
			camera_frame = 1.0f;
		}
		XMFLOAT2 siz{};
		for (int i = 0; i < 6; i++) {
			siz.x = Ease(In, Quad, camera_frame, 128, 64);
			siz.y = Ease(In, Quad, camera_frame, 128, 64);
			Camecon[i]->SetSize(siz);
		}
		base.x = Ease(In, Quad, camera_frame, 640, 90);
		base.y = Ease(In, Quad, camera_frame, 530, 680);
		Camecon[0]->SetPosition({ base.x + siz.x,base.y });
		Camecon[1]->SetPosition({ base.x + siz.x,base.y });
		Camecon[2]->SetPosition({ base.x + siz.x,base.y });
		Camecon[3]->SetPosition({ base.x - siz.x,base.y });
		Camecon[4]->SetPosition({ base.x - siz.x,base.y });
		Camecon[5]->SetPosition({ base.x       ,base.y });

	}
}


bool FirstStage::ClearUpdate() {
	if (stage_clear) {
		ResultCamera(intro_count/intro_count_max);
		intro_count++;
		ActorManager::GetInstance()->ResultUpdate(intro_count / intro_count_max);
		const float rnd_vel = 0.5f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		partMan->Add(100, goal_shadow->GetPosition(), vel, XMFLOAT3(), 1.2f, 0.0f, { 1,1,0.5f,1 }, { 1,1,1,0.3f });
		partMan->Update();
		if (input->TriggerButton(Input::A) || input->TriggerButton(Input::B)) {
			scene_changer->ChangeStart();
		}
		scene_changer->ChangeScene("MSECOND");
		FieldUpdate();
		return true;
	}
	if (enemy_shadow->GetPause()) {
		const float rnd_vel = 0.4f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		partMan->Add(120, enemy_shadow->GetPosition(), vel, XMFLOAT3(), 1.2f, 0.0f, { 1,1,1,1 }, { 1,1,1,0 });
		partMan->Update();

		finishTime++;

		if (finishTime > 200) {
			enemy_shadow->SetPause(false);
			enemy_shadow->SetCommand(Actor::DEAD);
		}
		return true;
	}
	if (enemy_shadow->GetCommand() == Actor::DEAD) {
		goal_shadow->SetIsActive(true);
		if (goal_shadow->GetPause() || player_shadow->GetPause()) {
			battle_result = true;
			stage_clear = true;
		}
	}
	return false;
}


void FirstStage::GameOverUpdate() {
	if (ActorManager::GetInstance()->SearchNum("Bullet") <= 0) {
		gameover = true;
		if (input->TriggerButton(Input::A)) {
			SceneManager::GetInstance()->ChangeScene("TITLE");
		}
	}
}

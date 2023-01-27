#include "BattleScene.h"


void BattleScene::BattleInit() {
	//導入枠
	Sprite* _Screen = nullptr;
	_Screen = Sprite::Create(ImageManager::SceneCover, { 0,0 });
	screens[0].reset(_Screen);
	Sprite* _Screen2 = nullptr;
	_Screen2 = Sprite::Create(ImageManager::SceneCover, { 0,600 });
	screens[1].reset(_Screen2);

	//ポーズUIを生成
	PauseUI* pause_ui = new PauseUI();
	pauseUi.reset(pause_ui);

	//暗転画面生成
	Sprite* first_ = Sprite::Create(ImageManager::Black, { 0.0f,0.0f });
	filter_first.reset(first_);
	filter_first->SetColor({ 1,1,1,filter_alpha });

	//カメラの操作説明
	hud = std::make_unique<Hud>();

	//ステージの生成します
	Object3d* skydome_{};
	skydome_ = new Object3d();
	skydome_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::kSkydome));
	skydome_->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
	skydome_->SetColor({ 0.3f,0.3f,0.3f,1.0f });
	skydome_->Initialize();
	skydome.reset(skydome_);

	Object3d* ground_{};
	ground_ = new Object3d();
	ground_->SetModel(ModelManager::GetIns()->GetModel(ModelManager::kDungeon));
	ground_->SetPosition(XMFLOAT3(-50, -0.5f, 50));
	ground_->SetScale(XMFLOAT3(5, 5, 5));
	ground_->Initialize();
	ground.reset(ground_);

}

void BattleScene::FieldUpdate() {
	//オブジェクトを改定して空を動かします
	float rot = skydome->GetRotation().y;
	const float vel = 0.1f;
	rot += vel;
	skydome->SetRotation({ 0,rot,0 });
	skydome->Update();
	//松明の更新処理
	for (std::unique_ptr<Touch>& torch : torchs) {
		torch->Update();
	}
	//フィールドの更新処理
	ground->Update();

}

void BattleScene::TorchSetup(int color) {
	std::ifstream file{};
	file.open("Resources/csv/torchpop.csv");
	assert(file.is_open());
	torch_pop << file.rdbuf();
	file.close();

	std::string line;
	while (getline(torch_pop, line)) {
		//解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//半角スペース区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');
		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//飛ばす
			continue;
		}
		//各コマンド
		if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			XMFLOAT3 pos{};
			pos.x = (float)std::atof(word.c_str());
			getline(line_stream, word, ',');
			pos.y = (float)std::atof(word.c_str());
			getline(line_stream, word, ',');
			pos.z = (float)std::atof(word.c_str());
			getline(line_stream, word, ',');
			if (word.find("DIR") == 0) {
				getline(line_stream, word, ',');
				float rot = 0;
				if (word.find("FRONT") == 0) {
					rot = 0;
				} else if (word.find("BACK") == 0) {
					rot = DEGREE_HALF;
				} else if (word.find("RIGHT") == 0) {
					rot = DEGREE_QUARTER;
				} else if (word.find("LEFT") == 0) {
					rot = -DEGREE_QUARTER;
				}
				getline(line_stream, word, ',');
				std::unique_ptr<Touch> new_torch;
				new_torch.reset(new Touch(pos, { 0,rot,0 }));
				new_torch->SetColor(color);
				torchs.push_back(std::move(new_torch));
			}
		}
		if (word.find("END") == 0) {
			break;
		}
	}
}

void BattleScene::CameraUpda() {
	//カメラリセット処理中
	if (reset_camera) {
		ResetCamera();
		return;
	}
	//カメラ回転入力
	if (input->TiltPushStick(Input::R_RIGHT) || input->TiltPushStick(Input::R_LEFT)) {
		if (!camera_explanation) {camera_explanation = true;}
		//スティック入力
		int reverse = pauseUi->GetReverseCamera();
		if (input->TiltPushStick(Input::R_RIGHT)) {
			camera_angle -= camera_vel * reverse;
		}
		if (input->TiltPushStick(Input::R_LEFT)) {
			camera_angle += camera_vel * reverse;
		}
		//負の整数をなくします
		if (camera_angle > DEGREE_MAX || camera_angle < 0) {
			camera_angle += DEGREE_MAX;
			camera_angle = (float)((int)camera_angle % (int)DEGREE_MAX);
		}
		//軽く補間をかけます
		e_camera_distance.x = sinf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;
		e_camera_distance.z = cosf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;

		const float interpolation = 0.75f;
		camera_distance.x = Ease(In, Quad, interpolation, camera_distance.x, e_camera_distance.x);
		camera_distance.z = Ease(In, Quad, interpolation, camera_distance.z, e_camera_distance.z);
	}
	//カメラリセット入力
	if (input->TriggerButton(Input::LT)) {
		if (!reset_camera) {
			if (!camera_explanation) {camera_explanation = true;}

			reset_camera = true;
			s_camera_distance = camera_distance;
			angle_frame = 0.0f;
			s_camera_angle = camera_angle;
			e_camera_angle = player_shadow->GetRotation().y;

			//始点終点の最短角度にします。
			if (abs(e_camera_angle - s_camera_angle) >= DEGREE_HALF) {
				if (e_camera_angle > s_camera_angle) {
					e_camera_angle -= DEGREE_MAX;
				} else {
					s_camera_angle -= DEGREE_MAX;
				}
			}
		}
	}
	player_shadow->SetAngle(camera_angle);
	if (player_shadow->GetHitBound()) { camera_hight = RandHeight(camera_hight); }
	else { camera_hight = kCameraHight; }
	//ターゲットをプレイヤーの正面ベクトルに合わせます
	camera->SetTarget(player_shadow->GetCameraPos(camera_angle, camera_target));
	camera->SetEye({ 
		player_shadow->GetPosition().x + camera_distance.x,
		player_shadow->GetPosition().y + camera_hight,
		player_shadow->GetPosition().z + camera_distance.z });
	camera->Update();
}

void BattleScene::ResetCamera() {
	//プレイヤーを動けなくします
	player_shadow->SetCanMove(false);

	if (angle_frame < 1.0f) {
		angle_frame += 1.0f / angle_frame_max;
	} else {
		reset_camera = false;
		player_shadow->SetCanMove(true);
		angle_frame = 1.0f;
	}

	camera_angle = Ease(In, Linear, angle_frame, s_camera_angle, e_camera_angle);

	XMFLOAT3 e_camera_distance{};
	e_camera_distance.x = sinf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;
	e_camera_distance.z = cosf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;

	camera_distance.x = Ease(In, Linear, angle_frame, s_camera_distance.x, e_camera_distance.x);
	camera_distance.z = Ease(In, Linear, angle_frame, s_camera_distance.z, e_camera_distance.z);

	camera->SetTarget(player_shadow->GetCameraPos(camera_angle, camera_target));
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + camera_distance.x,player_shadow->GetPosition().y + camera_hight,player_shadow->GetPosition().z + camera_distance.z });
	camera->Update();
}

bool BattleScene::PauseUpdate() {
	if (pause) {
		pauseUi->Update();
		if (pauseUi->GetEndFlag()) {
			pause = false;
		}
		return true;
	}
	if (input->TriggerButton(input->START)) {
		pause = true;
		if (pauseUi->GetEndFlag()) {
			pauseUi->SetEndFlag(false);
		}
	}
	return false;
}



float BattleScene::RandHeight(const float& base) {
	const float& rnd_vel = 0.95f;
	float Rand = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	float itr = 0;
	itr = base + Rand;

	return itr;
}
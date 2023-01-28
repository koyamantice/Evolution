#include "BattleScene.h"


void BattleScene::BattleInit() {
	//�����g
	Sprite* _Screen = nullptr;
	_Screen = Sprite::Create(ImageManager::SceneCover, { 0,0 });
	screens[0].reset(_Screen);
	Sprite* _Screen2 = nullptr;
	_Screen2 = Sprite::Create(ImageManager::SceneCover, { 0,600 });
	screens[1].reset(_Screen2);

	//�|�[�YUI�𐶐�
	PauseUI* pause_ui = new PauseUI();
	pauseUi.reset(pause_ui);

	//���肠����
	Sprite* _clear = nullptr;
	_clear = Sprite::Create(ImageManager::Clear, { 0,0 });
	clear_comment_.reset(_clear);
	//�Q�[���I�[�o�[����
	Sprite* _Over = nullptr;
	_Over = Sprite::Create(ImageManager::Over, { 0,0 });
	over_comment_.reset(_Over);


	//�Ó]��ʐ���
	Sprite* first_ = Sprite::Create(ImageManager::Black, { 0.0f,0.0f });
	filter_first.reset(first_);
	filter_first->SetColor({ 1,1,1,filter_alpha });

	//�J�����̑������
	hud = std::make_unique<Hud>();

	//�X�e�[�W�̐������܂�
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
	//�I�u�W�F�N�g�����肵�ċ�𓮂����܂�
	float rot = skydome->GetRotation().y;
	const float vel = 0.1f;
	rot += vel;
	skydome->SetRotation({ 0,rot,0 });
	skydome->Update();
	//�����̍X�V����
	for (std::unique_ptr<Touch>& torch : torchs) {
		torch->Update();
	}
	//�t�B�[���h�̍X�V����
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
		//��͂��₷������
		std::istringstream line_stream(line);

		std::string word;
		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		getline(line_stream, word, ',');
		//"//"����n�܂�s�̓R�����g
		if (word.find("//") == 0) {
			//��΂�
			continue;
		}
		//�e�R�}���h
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
	//�J�������Z�b�g������
	if (reset_camera) {
		ResetCamera();
		return;
	}
	//�J������]����
	if (input->TiltPushStick(Input::R_RIGHT) || input->TiltPushStick(Input::R_LEFT)) {
		if (!camera_explanation) {camera_explanation = true;}
		//�X�e�B�b�N����
		int reverse = pauseUi->GetReverseCamera();
		if (input->TiltPushStick(Input::R_RIGHT)) {
			camera_angle -= camera_vel * reverse;
		}
		if (input->TiltPushStick(Input::R_LEFT)) {
			camera_angle += camera_vel * reverse;
		}
		//���̐������Ȃ����܂�
		if (camera_angle > DEGREE_MAX || camera_angle < 0) {
			camera_angle += DEGREE_MAX;
			camera_angle = (float)((int)camera_angle % (int)DEGREE_MAX);
		}
		//�y����Ԃ������܂�
		e_camera_distance.x = sinf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;
		e_camera_distance.z = cosf(camera_angle * (XM_PI / DEGREE_HALF)) * camera_radius;

		const float interpolation = 0.75f;
		camera_distance.x = Ease(In, Quad, interpolation, camera_distance.x, e_camera_distance.x);
		camera_distance.z = Ease(In, Quad, interpolation, camera_distance.z, e_camera_distance.z);
	}
	//�J�������Z�b�g����
	if (input->TriggerButton(Input::LT)) {
		if (!reset_camera) {
			if (!camera_explanation) {camera_explanation = true;}

			reset_camera = true;
			s_camera_distance = camera_distance;
			angle_frame = 0.0f;
			s_camera_angle = camera_angle;
			e_camera_angle = player_shadow->GetRotation().y;

			//�n�_�I�_�̍ŒZ�p�x�ɂ��܂��B
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
	//�^�[�Q�b�g���v���C���[�̐��ʃx�N�g���ɍ��킹�܂�
	camera->SetTarget(player_shadow->GetCameraPos(camera_angle, camera_target));
	camera->SetEye({ 
		player_shadow->GetPosition().x + camera_distance.x,
		player_shadow->GetPosition().y + camera_hight,
		player_shadow->GetPosition().z + camera_distance.z });
	camera->Update();
}

void BattleScene::ResetCamera() {
	//�v���C���[�𓮂��Ȃ����܂�
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

void BattleScene::BattleBackDraw() {
	Object3d::PreDraw();
	skydome->Draw();
	ground->Draw();
	for (std::unique_ptr<Touch>& torch : torchs) {
		torch->Draw();
	}
}

void BattleScene::BattleFrontDraw(blendType _blendType,Sprite* _introword) {
	particleEmitter->Draw(_blendType);
	Sprite::PreDraw();
	if (battle_intro || battle_result) {
		screens[0]->Draw();
		screens[1]->Draw();
		if (battle_intro) {
			if (_introword !=nullptr) {
				_introword->Draw();
			}
		}
		if (stage_clear) {
			clear_comment_->Draw();
		}
	} else {
		if (!stage_clear) {
			DrawLocal();
			hud->Draw();
		}
	}
	if (gameover) {
		over_comment_->Draw();
	}
	if (scene_first_change) {
		filter_first->Draw();
	}
	if (pause) {
		pauseUi->Draw();
		return;
	}

	scene_changer->Draw();
}


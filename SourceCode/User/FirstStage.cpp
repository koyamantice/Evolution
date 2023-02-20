#include "FirstStage.h"

void FirstStage::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	BattleInit();
	TorchSetup(Touch::FireColor::f_green);

	//�Q�[���A�N�^�[�̐��������܂��B
	ActorManager::GetInstance()->AttachActor("Player");
	ActorManager::GetInstance()->AttachActor("MashGhost");
	ActorManager::GetInstance()->AttachActor("ClearCrystal");

	for (int i = 0; i < kGnormNum; i++) {
		ActorManager::GetInstance()->AttachBullet("Red");
	}
	for (int i = 0; i < kGnormNum; i++) {
		ActorManager::GetInstance()->AttachBullet("Green");
	}
	//�V�[�����ŕK�v�ȃA�N�^�[���Q�Ƃ��܂��B
	player_shadow = ActorManager::GetInstance()->SearchActor("Player");
	enemy_shadow = ActorManager::GetInstance()->SearchActor("Enemy");
	goal_shadow = ActorManager::GetInstance()->SearchActor("ClearCrystal");
	goal_shadow->SetPosition(enemy_shadow->GetPosition());
	goal_shadow->SetIsActive(false);

	//���������̌��t
	for(int i=0;i<intro_word_max;i++){ 
		Sprite* IntroWord_ = Sprite::Create(ImageManager::Intro01 + i, { 1230.0f,600.0f }, { 1,1,1,1 }, { 1.0f, 0 });
		IntroWord[i].reset(IntroWord_);	
	}

	//�J�����̏�����
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

	//�|�X�g�G�t�F�N�g�̏�����
	postEffect = new PostEffect();
	postEffect->Initialize();
	//
	audioManager = std::make_unique<AudioManager>();

	audioManager->LoadWave("BGM/battle.wav");
	audioManager->PlayWave("BGM/battle.wav", 0.5f);

	//�p�[�e�B�N���̏�����
	particleEmitter = std::make_unique <ParticleEmitter>(ImageManager::charge);
}
//�X�V
void FirstStage::Update(DirectXCommon* dxCommon) {
	//���������̏���
	if (IntroUpdate()) { return; }
	//�N���A��̏���
	if (ClearUpdate()) { return; }
	//�|�[�Y�����̍X�V
	if (PauseUpdate()) { return; }
	//�Q�[���I�[�o�[�̏���
	GameOverUpdate();
	//�A�N�^�[���ׂĂ̍X�V����
	ActorManager::GetInstance()->Update();
	//�J�����̍X�V����
	CameraUpda();
	//�X�e�[�W�̍X�V����
	FieldUpdate();
	//��������̍X�V����
	hud->Update();
	//�p�[�e�B�N���̍X�V����
	particleEmitter->Update();
}

//�`��
void FirstStage::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	//postEffect->PreDrawScene(dxCommon->GetCmdList());
	BattleBackDraw();
	//�w�i�p
	ActorManager::GetInstance()->Draw(dxCommon);
	BattleFrontDraw(alphaBle,IntroWord[nowWord].get());
	dxCommon->PostDraw();
}

//�J������
void FirstStage::Finalize() {
	//�R���̃��f���̃f���[�g
	ActorManager::GetInstance()->Finalize();
}

bool FirstStage::IntroUpdate() {
	if (battle_intro) {
		//�t�F�[�h�C���@�\
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

		//�X�L�b�v
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

		//���������I������
		if (intro_count >= intro_count_max) {
			intro_count = 0;
			battle_intro = false;
			return true;
		}

		//�����̌��𕪊����Ă��܂�.
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
	//���n���Ă���̗P�\
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
	camera->SetTarget(goal_shadow->GetPosition());
	camera->SetEye(XMFLOAT3{ player_shadow->GetPosition().x + camera_distance.x,player_shadow->GetPosition().y + camera_hight,player_shadow->GetPosition().z + camera_distance.z });
	camera->Update();
}


bool FirstStage::ClearUpdate() {
	if (stage_clear) {
		const float rnd_vel = 0.5f;
		particleEmitter->AddCommon(100, goal_shadow->GetPosition(), rnd_vel, 0.0f, 1.2f, 0.0f, { 1,1,0.5f,1 }, { 1,1,1,0.3f });
		particleEmitter->Update();

		if (input->TriggerButton(Input::A) || input->TriggerButton(Input::B)) {
			scene_changer->ChangeStart();
		}
		ResultCamera(0);
		scene_changer->ChangeScene("MSECOND");
		ActorManager::GetInstance()->ResultUpdate(0);
		FieldUpdate();
		return true;
	}
	if (enemy_shadow->GetPause()) {
		const float rnd_vel = 0.4f;
		particleEmitter->AddCommon(120, enemy_shadow->GetPosition(), rnd_vel, 0, 1.2f, 0.0f, { 1,1,1,1 }, { 1,1,1,0 });
		particleEmitter->Update();
		finish_time++;

		if (finish_time > finish_time_Max) {
			enemy_shadow->SetPause(false);
			enemy_shadow->SetCanMove(false);
		}
		return true;
	}
	if (finish_time > finish_time_Max) {
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

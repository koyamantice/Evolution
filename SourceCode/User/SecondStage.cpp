#include"SecondStage.h"

void SecondStage::Initialize(DirectXCommon* dxCommon) {
	InitCommon(dxCommon);
	BattleInit();
	TorchSetup(Touch::FireColor::f_blue);

	//�Q�[���A�N�^�[�̐��������܂��B
	ActorManager::GetInstance()->AttachActor("Player");
	ActorManager::GetInstance()->AttachActor("Enemy_Bee");
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

	//�J�����̏�����
	camera_distance.x = sinf(camera_angle * (XM_PI / 180)) * camera_radius;
	camera_distance.z = cosf(camera_angle * (XM_PI / 180)) * camera_radius;
	player_shadow->SetAngle(camera_angle);
	camera->SetTarget(first_target);
	camera->SetEye(XMFLOAT3{
		player_shadow->GetPosition().x + camera_distance.x,
		first_hight,
		player_shadow->GetPosition().z + camera_distance.z
		});
	camera->Update();

	//�|�X�g�G�t�F�N�g�̏�����
	postEffect = new PostEffect();
	postEffect->Initialize();

	//�p�[�e�B�N���̏�����
	particleEmitter = std::make_unique <ParticleEmitter>(ImageManager::charge);
}

//�X�V
void SecondStage::Update(DirectXCommon* dxCommon) {
	if (IntroUpdate()) { return; }
	//�N���A��̏���
	if (ClearUpdate()) { return; }
	//�|�[�Y�����̍X�V
	if (PauseUpdate()) { return; }
	//�Q�[���I�[�o�[�̏���
	GameOverUpdate();
	//�N���A��̏���
	if (ClearUpdate()) { return; }
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
void SecondStage::Draw(DirectXCommon* dxCommon) {
	dxCommon->PreDraw();
	//postEffect->PreDrawScene(dxCommon->GetCmdList());
	BattleBackDraw();
	//�w�i�p
	ActorManager::GetInstance()->Draw(dxCommon);
	BattleFrontDraw();
	dxCommon->PostDraw();

}
//�J������
void SecondStage::Finalize() {
	//�R���̃��f���̃f���[�g
	ActorManager::GetInstance()->Finalize();
}

bool SecondStage::IntroUpdate() {
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
			ActorManager::GetInstance()->IntroUpdate(intro_count,"", kSecondScene);
			FieldUpdate();
			return true;
		}
		intro_count += intro_speed;

		//���������I������
		if (intro_count >= intro_count_max) {
			intro_count = 0;
			battle_intro = false;
			return true;
		}

		IntroCamera(intro_count);
		ActorManager::GetInstance()->IntroUpdate(intro_count / intro_count_max,"", kSecondScene);
		FieldUpdate();
		return true;
	}
	return false;
}
void SecondStage::IntroCamera(const float& Timer) {




}
void SecondStage::ResultCamera(const float& Timer) {
}


bool SecondStage::ClearUpdate() {
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
		particleEmitter->AddCommon(120, enemy_shadow->GetPosition(), rnd_vel, 0, 1.2f, 0.0f, { 1,1,0,1 }, { 1,1,0,0 });
		particleEmitter->Update();
		finish_time++;

		if (finish_time > finish_time_Max) {
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
void SecondStage::GameOverUpdate() {
}
